/*
 * Copyright (c) 2010-2024 Belledonne Communications SARL.
 *
 * This file is part of linphone-desktop
 * (see https://www.linphone.org).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "tool/LinphoneEnums.hpp"

#include "App.hpp"

#include <QCoreApplication>
#include <QDirIterator>
#include <QFileSelector>
#include <QFontDatabase>
#include <QGuiApplication>
#include <QLibraryInfo>
#include <QProcessEnvironment>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQmlFileSelector>
#include <QQuickWindow>
#include <QTimer>

#include "core/account/AccountCore.hpp"
#include "core/account/AccountProxy.hpp"
#include "core/call-history/CallHistoryProxy.hpp"
#include "core/call/CallCore.hpp"
#include "core/call/CallGui.hpp"
#include "core/call/CallList.hpp"
#include "core/call/CallProxy.hpp"
#include "core/camera/CameraGui.hpp"
#include "core/conference/ConferenceGui.hpp"
#include "core/conference/ConferenceInfoGui.hpp"
#include "core/conference/ConferenceInfoProxy.hpp"
#include "core/fps-counter/FPSCounter.hpp"
#include "core/friend/FriendCore.hpp"
#include "core/friend/FriendGui.hpp"
#include "core/logger/QtLogger.hpp"
#include "core/login/LoginPage.hpp"
#include "core/notifier/Notifier.hpp"
#include "core/participant/ParticipantDeviceCore.hpp"
#include "core/participant/ParticipantDeviceProxy.hpp"
#include "core/participant/ParticipantGui.hpp"
#include "core/participant/ParticipantProxy.hpp"
#include "core/phone-number/PhoneNumber.hpp"
#include "core/phone-number/PhoneNumberProxy.hpp"
#include "core/register/RegisterPage.hpp"
#include "core/screen/ScreenList.hpp"
#include "core/screen/ScreenProxy.hpp"
#include "core/search/MagicSearchProxy.hpp"
#include "core/setting/SettingsCore.hpp"
#include "core/singleapplication/singleapplication.h"
#include "core/timezone/TimeZone.hpp"
#include "core/timezone/TimeZoneProxy.hpp"
#include "core/variant/VariantList.hpp"
#include "core/videoSource/VideoSourceDescriptorGui.hpp"
#include "model/object/VariantObject.hpp"
#include "tool/Constants.hpp"
#include "tool/EnumsToString.hpp"
#include "tool/Utils.hpp"
#include "tool/native/DesktopTools.hpp"
#include "tool/providers/AvatarProvider.hpp"
#include "tool/providers/ImageProvider.hpp"
#include "tool/providers/ScreenProvider.hpp"
#include "tool/request/RequestDialog.hpp"
#include "tool/thread/Thread.hpp"

DEFINE_ABSTRACT_OBJECT(App)

#ifdef Q_OS_LINUX
const QString ApplicationsDirectory(QDir::homePath().append(QStringLiteral("/.local/share/applications/")));
const QString IconsDirectory(QDir::homePath().append(QStringLiteral("/.local/share/icons/hicolor/scalable/apps/")));
#endif

App::App(int &argc, char *argv[])
    : SingleApplication(argc, argv, true, Mode::User | Mode::ExcludeAppPath | Mode::ExcludeAppVersion) {
	// Do not use APPLICATION_NAME here.
	// The EXECUTABLE_NAME will be used in qt standard paths. It's our goal.

	QCoreApplication::setApplicationName(EXECUTABLE_NAME);
	QApplication::setOrganizationDomain(EXECUTABLE_NAME);
	QCoreApplication::setApplicationVersion(APPLICATION_SEMVER);
	// If not OpenGL, createRender is never call.
	QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
	// Ignore vertical sync. This way, we avoid blinking on resizes(and other refresh like layouts etc.).
	auto ignoreVSync = QSurfaceFormat::defaultFormat();
	ignoreVSync.setSwapInterval(0);
	QSurfaceFormat::setDefaultFormat(ignoreVSync);
	setWindowIcon(QIcon(Constants::WindowIconPath));
	lInfo() << "Loading Fonts";
	QDirIterator it(":/font/", QDirIterator::Subdirectories);
	while (it.hasNext()) {
		QString ttf = it.next();
		// lDebug()<< ttf;
		auto id = QFontDatabase::addApplicationFont(ttf);
	}

	//-------------------
	mLinphoneThread = new Thread(this);

	init();
	lInfo() << QStringLiteral("Starting application " APPLICATION_NAME " (bin: " EXECUTABLE_NAME
	                          "). Version:%1 Os:%2 Qt:%3")
	               .arg(applicationVersion())
	               .arg(Utils::getOsProduct())
	               .arg(qVersion());
}

App::~App() {
}

void App::setSelf(QSharedPointer<App>(me)) {
	mCoreModelConnection = QSharedPointer<SafeConnection<App, CoreModel>>(
	    new SafeConnection<App, CoreModel>(me, CoreModel::getInstance()), &QObject::deleteLater);
	mCoreModelConnection->makeConnectToModel(&CoreModel::callCreated,
	                                         [this](const std::shared_ptr<linphone::Call> &call) {
		                                         if (call->getDir() == linphone::Call::Dir::Incoming) return;
		                                         auto callCore = CallCore::create(call);
		                                         mCoreModelConnection->invokeToCore([this, callCore] {
			                                         auto callGui = new CallGui(callCore);
			                                         auto win = getCallsWindow(QVariant::fromValue(callGui));
			                                         Utils::smartShowWindow(win);
			                                         lDebug() << "App : call created" << callGui;
		                                         });
	                                         });
	mCoreModelConnection->makeConnectToModel(&CoreModel::requestRestart, [this]() {
		mCoreModelConnection->invokeToCore([this]() {
			lInfo() << log().arg("Restarting");
			restart();
		});
	});
	mCoreModelConnection->makeConnectToModel(&CoreModel::requestFetchConfig, [this](QString path) {
		mCoreModelConnection->invokeToCore([this, path]() {
			auto callback = [this, path]() {
				RequestDialog *obj = new RequestDialog(
				    tr("Voulez-vous télécharger et appliquer la configuration depuis cette adresse ?"), path);
				connect(obj, &RequestDialog::result, this, [this, obj, path](int result) {
					if (result == 1) {
						mCoreModelConnection->invokeToModel(
						    [this, path]() { CoreModel::getInstance()->setFetchConfig(path); });
					} else if (result == 0) {
						mCoreModelConnection->invokeToModel([]() { CliModel::getInstance()->resetProcesses(); });
					}
					obj->deleteLater();
				});
				QMetaObject::invokeMethod(getMainWindow(), "showConfirmationPopup", QVariant::fromValue(obj));
			};
			if (!getMainWindow()) { // Delay
				connect(this, &App::mainWindowChanged, this, callback, Qt::SingleShotConnection);
			} else {
				callback();
			}
		});
	});
	//---------------------------------------------------------------------------------------------
	mCliModelConnection = QSharedPointer<SafeConnection<App, CliModel>>(
	    new SafeConnection<App, CliModel>(me, CliModel::getInstance()), &QObject::deleteLater);
	mCliModelConnection->makeConnectToCore(&App::receivedMessage, [this](int, const QByteArray &byteArray) {
		QString command(byteArray);
		if (command.isEmpty())
			mCliModelConnection->invokeToModel([command]() { CliModel::getInstance()->runProcess(); });
		else {
			qInfo() << QStringLiteral("Received command from other application: `%1`.").arg(command);
			mCliModelConnection->invokeToModel([command]() { CliModel::getInstance()->executeCommand(command); });
		}
	});
	mCliModelConnection->makeConnectToModel(&CliModel::showMainWindow, [this]() {
		mCliModelConnection->invokeToCore([this]() { Utils::smartShowWindow(getMainWindow()); });
	});
}

App *App::getInstance() {
	return dynamic_cast<App *>(QApplication::instance());
}

Notifier *App::getNotifier() const {
	return mNotifier;
}
//-----------------------------------------------------------
//		Initializations
//-----------------------------------------------------------

void App::init() {
	// Console Commands
	createCommandParser();
	mParser->parse(this->arguments());
	// TODO : Update languages for command translations.

	createCommandParser(); // Recreate parser in order to use translations from config.
	mParser->process(*this);

	if (!mLinphoneThread->isRunning()) {
		lDebug() << log().arg("Starting Thread");
		mLinphoneThread->start();
	}
	setQuitOnLastWindowClosed(true); // TODO: use settings to set it

	lInfo() << log().arg("Display server : %1").arg(platformName());
}

void App::initCore() {
	// Core. Manage the logger so it must be instantiate at first.
	CoreModel::create("", mLinphoneThread);
	if (mParser->isSet("verbose")) QtLogger::enableVerbose(true);
	if (mParser->isSet("qt-logs-only")) QtLogger::enableQtOnly(true);
	QMetaObject::invokeMethod(
	    mLinphoneThread->getThreadId(),
	    [this]() mutable {
		    CoreModel::getInstance()->start();
		    auto settings = Settings::create();
		    QMetaObject::invokeMethod(App::getInstance()->thread(), [this, settings]() mutable {
			    // QML
			    mEngine = new QQmlApplicationEngine(this);
			    // Provide `+custom` folders for custom components and `5.9` for old components.
			    QStringList selectors("custom");
			    const QVersionNumber &version = QLibraryInfo::version();
			    if (version.majorVersion() == 5 && version.minorVersion() == 9) selectors.push_back("5.9");
			    auto selector = new QQmlFileSelector(mEngine, mEngine);
			    selector->setExtraSelectors(selectors);
			    lInfo() << log().arg("Activated selectors:") << selector->selector()->allSelectors();

			    mEngine->addImportPath(":/");
			    mEngine->rootContext()->setContextProperty("applicationDirPath", QGuiApplication::applicationDirPath());
#ifdef APPLICATION_VENDOR
			    mEngine->rootContext()->setContextProperty("applicationVendor", APPLICATION_VENDOR);
#else
				mEngine->rootContext()->setContextProperty("applicationVendor", "");
#endif
#ifdef APPLICATION_LICENCE
			    mEngine->rootContext()->setContextProperty("applicationLicence", APPLICATION_LICENCE);
#else
				mEngine->rootContext()->setContextProperty("applicationLicence", "");
#endif
#ifdef APPLICATION_LICENCE_URL
			    mEngine->rootContext()->setContextProperty("applicationLicenceUrl", APPLICATION_LICENCE_URL);
#else
				mEngine->rootContext()->setContextProperty("applicationLicenceUrl", "");
#endif
#ifdef COPYRIGHT_RANGE_DATE
			    mEngine->rootContext()->setContextProperty("copyrightRangeDate", COPYRIGHT_RANGE_DATE);
#else
				mEngine->rootContext()->setContextProperty("copyrightRangeDate", "");
#endif
			    mEngine->rootContext()->setContextProperty("applicationName", APPLICATION_NAME);
			    mEngine->rootContext()->setContextProperty("executableName", EXECUTABLE_NAME);

			    initCppInterfaces();
			    mEngine->addImageProvider(ImageProvider::ProviderId, new ImageProvider());
			    mEngine->addImageProvider(AvatarProvider::ProviderId, new AvatarProvider());
			    mEngine->addImageProvider(ScreenProvider::ProviderId, new ScreenProvider());
			    mEngine->addImageProvider(WindowProvider::ProviderId, new WindowProvider());
			    mEngine->addImageProvider(WindowIconProvider::ProviderId, new WindowIconProvider());

			    // Enable notifications.
			    mNotifier = new Notifier(mEngine);
			    mSettings = settings;
			    settings.reset();

			    const QUrl url(u"qrc:/Linphone/view/App/Main.qml"_qs);
			    QObject::connect(
			        mEngine, &QQmlApplicationEngine::objectCreated, this,
			        [this, url](QObject *obj, const QUrl &objUrl) {
				        if (url == objUrl) {
					        if (!obj) {
						        lCritical() << log().arg("Main.qml couldn't be load. The app will exit");
						        exit(-1);
					        }
					        setMainWindow(qobject_cast<QQuickWindow *>(obj));
					        QMetaObject::invokeMethod(obj, "initStackViewItem");
					        Q_ASSERT(mMainWindow);
				        }
			        },
			        Qt::QueuedConnection);
			    mEngine->load(url);
		    });
		    // coreModel.reset();
	    },
	    Qt::BlockingQueuedConnection);
}

void App::initCppInterfaces() {
	qmlRegisterSingletonType<LoginPage>(
	    Constants::MainQmlUri, 1, 0, "LoginPageCpp", [](QQmlEngine *engine, QJSEngine *) -> QObject * {
		    static auto loginPage = new LoginPage(engine);
		    App::getInstance()->mEngine->setObjectOwnership(loginPage, QQmlEngine::CppOwnership);
		    return loginPage;
	    });
	qmlRegisterSingletonType<RegisterPage>(
	    Constants::MainQmlUri, 1, 0, "RegisterPageCpp", [](QQmlEngine *engine, QJSEngine *) -> QObject * {
		    static RegisterPage *registerPage = new RegisterPage();
		    App::getInstance()->mEngine->setObjectOwnership(registerPage, QQmlEngine::CppOwnership);
		    return registerPage;
	    });
	qmlRegisterSingletonType<Constants>(
	    "ConstantsCpp", 1, 0, "ConstantsCpp",
	    [](QQmlEngine *engine, QJSEngine *) -> QObject * { return new Constants(engine); });
	qmlRegisterSingletonType<Utils>("UtilsCpp", 1, 0, "UtilsCpp",
	                                [](QQmlEngine *engine, QJSEngine *) -> QObject * { return new Utils(engine); });
	qmlRegisterSingletonType<DesktopTools>(
	    "DesktopToolsCpp", 1, 0, "DesktopToolsCpp",
	    [](QQmlEngine *engine, QJSEngine *) -> QObject * { return new DesktopTools(engine); });
	qmlRegisterSingletonType<EnumsToString>(
	    "EnumsToStringCpp", 1, 0, "EnumsToStringCpp",
	    [](QQmlEngine *engine, QJSEngine *) -> QObject * { return new EnumsToString(engine); });

	qmlRegisterSingletonType<Settings>(
	    "SettingsCpp", 1, 0, "SettingsCpp",
	    [this](QQmlEngine *engine, QJSEngine *) -> QObject * { return mSettings.get(); });

	qmlRegisterType<PhoneNumberProxy>(Constants::MainQmlUri, 1, 0, "PhoneNumberProxy");
	qmlRegisterType<VariantObject>(Constants::MainQmlUri, 1, 0, "VariantObject");
	qmlRegisterType<VariantList>(Constants::MainQmlUri, 1, 0, "VariantList");

	qmlRegisterType<ParticipantProxy>(Constants::MainQmlUri, 1, 0, "ParticipantProxy");
	qmlRegisterType<ParticipantGui>(Constants::MainQmlUri, 1, 0, "ParticipantGui");
	qmlRegisterType<ConferenceInfoProxy>(Constants::MainQmlUri, 1, 0, "ConferenceInfoProxy");
	qmlRegisterType<ConferenceInfoGui>(Constants::MainQmlUri, 1, 0, "ConferenceInfoGui");

	qmlRegisterType<PhoneNumberProxy>(Constants::MainQmlUri, 1, 0, "PhoneNumberProxy");
	qmlRegisterUncreatableType<PhoneNumber>(Constants::MainQmlUri, 1, 0, "PhoneNumber", QLatin1String("Uncreatable"));
	qmlRegisterType<AccountProxy>(Constants::MainQmlUri, 1, 0, "AccountProxy");
	qmlRegisterType<AccountGui>(Constants::MainQmlUri, 1, 0, "AccountGui");
	qmlRegisterUncreatableType<AccountCore>(Constants::MainQmlUri, 1, 0, "AccountCore", QLatin1String("Uncreatable"));
	qmlRegisterUncreatableType<CallCore>(Constants::MainQmlUri, 1, 0, "CallCore", QLatin1String("Uncreatable"));
	qmlRegisterType<CallProxy>(Constants::MainQmlUri, 1, 0, "CallProxy");
	qmlRegisterType<CallHistoryProxy>(Constants::MainQmlUri, 1, 0, "CallHistoryProxy");
	qmlRegisterType<CallGui>(Constants::MainQmlUri, 1, 0, "CallGui");
	qmlRegisterUncreatableType<ConferenceCore>(Constants::MainQmlUri, 1, 0, "ConferenceCore",
	                                           QLatin1String("Uncreatable"));
	qmlRegisterType<ConferenceGui>(Constants::MainQmlUri, 1, 0, "ConferenceGui");
	qmlRegisterType<FriendGui>(Constants::MainQmlUri, 1, 0, "FriendGui");
	qmlRegisterUncreatableType<FriendCore>(Constants::MainQmlUri, 1, 0, "FriendCore", QLatin1String("Uncreatable"));
	qmlRegisterType<MagicSearchProxy>(Constants::MainQmlUri, 1, 0, "MagicSearchProxy");
	qmlRegisterType<CameraGui>(Constants::MainQmlUri, 1, 0, "CameraGui");
	qmlRegisterType<FPSCounter>(Constants::MainQmlUri, 1, 0, "FPSCounter");

	qmlRegisterType<TimeZoneProxy>(Constants::MainQmlUri, 1, 0, "TimeZoneProxy");

	qmlRegisterType<ParticipantDeviceGui>(Constants::MainQmlUri, 1, 0, "ParticipantDeviceGui");
	qmlRegisterType<ParticipantDeviceProxy>(Constants::MainQmlUri, 1, 0, "ParticipantDeviceProxy");

	qmlRegisterUncreatableType<ScreenList>(Constants::MainQmlUri, 1, 0, "ScreenList", QLatin1String("Uncreatable"));
	qmlRegisterType<ScreenProxy>(Constants::MainQmlUri, 1, 0, "ScreenProxy");

	qmlRegisterUncreatableType<VideoSourceDescriptorCore>(Constants::MainQmlUri, 1, 0, "VideoSourceDescriptorCore",
	                                                      QLatin1String("Uncreatable"));
	qmlRegisterType<VideoSourceDescriptorGui>(Constants::MainQmlUri, 1, 0, "VideoSourceDescriptorGui");

	qmlRegisterUncreatableType<RequestDialog>(Constants::MainQmlUri, 1, 0, "RequestDialog",
	                                          QLatin1String("Uncreatable"));

	LinphoneEnums::registerMetaTypes();
}

//------------------------------------------------------------

void App::clean() {
	if (mSettings) mSettings.reset();
	// Wait 500ms to let time for log te be stored.
	// mNotifier destroyed in mEngine deletion as it is its parent
	delete mEngine;
	mEngine = nullptr;
	// if (mSettings) {
	// mSettings.reset();
	// }
	qApp->processEvents(QEventLoop::AllEvents, 500);
	if (mLinphoneThread) {
		mLinphoneThread->exit();
		mLinphoneThread->wait();
		delete mLinphoneThread;
	}
}
void App::restart() {
	mCoreModelConnection->invokeToModel([this]() {
		CoreModel::getInstance()->getCore()->stop();
		mCoreModelConnection->invokeToCore([this]() {
			mEngine->deleteLater();
			if (mSettings) mSettings.reset();
			initCore();
			// Retrieve self from current Core/Model connection and reset Qt connections.
			auto oldConnection = mCoreModelConnection;
			oldConnection->mCore.lock();
			auto me = oldConnection->mCore.mQData;
			setSelf(me);
			oldConnection->mCore.unlock();
			exit((int)StatusCode::gRestartCode);
		});
	});
}
void App::createCommandParser() {
	if (!mParser) delete mParser;

	mParser = new QCommandLineParser();
	mParser->setApplicationDescription(tr("applicationDescription"));
	mParser->addPositionalArgument("command", tr("commandLineDescription").replace("%1", APPLICATION_NAME),
	                               "[command]");
	mParser->addOptions({
	    {{"h", "help"}, tr("commandLineOptionHelp")},
	    {"cli-help", tr("commandLineOptionCliHelp").replace("%1", APPLICATION_NAME)},
	    {{"v", "version"}, tr("commandLineOptionVersion")},
	    {"config", tr("commandLineOptionConfig").replace("%1", EXECUTABLE_NAME), tr("commandLineOptionConfigArg")},
	    {"fetch-config", tr("commandLineOptionFetchConfig").replace("%1", EXECUTABLE_NAME),
	     tr("commandLineOptionFetchConfigArg")},
	    {{"c", "call"}, tr("commandLineOptionCall").replace("%1", EXECUTABLE_NAME), tr("commandLineOptionCallArg")},
#ifndef Q_OS_MACOS
	    {"iconified", tr("commandLineOptionIconified")},
#endif // ifndef Q_OS_MACOS
	    {{"V", "verbose"}, tr("commandLineOptionVerbose")},
	    {"qt-logs-only", tr("commandLineOptionQtLogsOnly")},
	});
}
// Should be call only at first start
void App::sendCommand() {
	auto arguments = mParser->positionalArguments();
	static bool firstStart = true; // We can't erase positional arguments. So we get them on each restart.
	if (firstStart && arguments.size() > 0) {
		firstStart = false;
		if (isSecondary()) { // Send to primary
			lDebug() << "Sending " << arguments;
			for (auto i : arguments) {
				sendMessage(i.toLocal8Bit(), -1);
			}
		} else { // Execute
			lDebug() << "Executing " << arguments;
			for (auto i : arguments) {
				QString command(i);
				receivedMessage(0, i.toLocal8Bit());
			}
		}
	} else if (isPrimary()) { // Run waiting process
		receivedMessage(0, "");
	}
}

bool App::notify(QObject *receiver, QEvent *event) {
	bool done = true;
	try {
		done = QApplication::notify(receiver, event);
	} catch (const std::exception &ex) {
		lCritical() << log().arg("Exception has been catch in notify");
	} catch (...) {
		lCritical() << log().arg("Generic exeption has been catch in notify");
	}
	return done;
}

QQuickWindow *App::getCallsWindow(QVariant callGui) {
	mustBeInMainThread(getClassName());
	if (!mCallsWindow) {
		const QUrl callUrl("qrc:/Linphone/view/App/CallsWindow.qml");

		lInfo() << log().arg("Creating subwindow: `%1`.").arg(callUrl.toString());

		QQmlComponent component(mEngine, callUrl);
		if (component.isError()) {
			qWarning() << component.errors();
			abort();
		}
		lInfo() << log().arg("Subwindow status: `%1`.").arg(component.status());

		QObject *object = nullptr;
		// if (!callGui.isNull() && callGui.isValid()) object = component.createWithInitialProperties({{"call",
		// callGui}});
		object = component.create();
		Q_ASSERT(object);
		if (!object) {
			lCritical() << log().arg("Calls window could not be created.");
			return nullptr;
		}

		// QQmlEngine::setObjectOwnership(object, QQmlEngine::CppOwnership);
		object->setParent(mEngine);

		auto window = qobject_cast<QQuickWindow *>(object);
		Q_ASSERT(window);
		if (!window) {
			lCritical() << log().arg("Calls window could not be created.");
			return nullptr;
		}
		// window->setParent(mMainWindow);
		mCallsWindow = window;
	}
	if (!callGui.isNull() && callGui.isValid()) mCallsWindow->setProperty("call", callGui);
	return mCallsWindow;
}

void App::setCallsWindowProperty(const char *id, QVariant property) {
	if (mCallsWindow) mCallsWindow->setProperty(id, property);
}

void App::closeCallsWindow() {
	if (mCallsWindow) {
		mCallsWindow->close();
		mCallsWindow->deleteLater();
		mCallsWindow = nullptr;
	}
}

QQuickWindow *App::getMainWindow() const {
	return mMainWindow;
}

void App::setMainWindow(QQuickWindow *data) {
	if (mMainWindow != data) {
		mMainWindow = data;
		emit mainWindowChanged();
	}
}

#ifdef Q_OS_LINUX
QString App::getApplicationPath() const {
	const QString binPath(QCoreApplication::applicationFilePath());

	// Check if installation is done via Flatpak, AppImage, or classic package
	// in order to rewrite a correct exec path for autostart
	QString exec;
	qDebug() << "binpath=" << binPath;
	if (binPath.startsWith("/app")) { // Flatpak
		exec = QStringLiteral("flatpak run " APPLICATION_ID);
		qDebug() << "exec path autostart set flatpak=" << exec;
	} else if (binPath.startsWith("/tmp/.mount")) { // Appimage
		exec = QProcessEnvironment::systemEnvironment().value(QStringLiteral("APPIMAGE"));
		qDebug() << "exec path autostart set appimage=" << exec;
	} else { // classic package
		exec = binPath;
		qDebug() << "exec path autostart set classic package=" << exec;
	}
	return exec;
}

void App::exportDesktopFile() {
	QDir dir(ApplicationsDirectory);
	if (!dir.exists() && !dir.mkpath(ApplicationsDirectory)) {
		qWarning() << QStringLiteral("Unable to build applications dir path: `%1`.").arg(ApplicationsDirectory);
		return;
	}

	const QString confPath(ApplicationsDirectory + EXECUTABLE_NAME ".desktop");
	if (generateDesktopFile(confPath, true, false)) generateDesktopFile(confPath, false, false);
}

bool App::generateDesktopFile(const QString &confPath, bool remove, bool openInBackground) {
	qInfo() << QStringLiteral("Updating `%1`...").arg(confPath);
	QFile file(confPath);

	if (remove) {
		if (file.exists() && !file.remove()) {
			qWarning() << QLatin1String("Unable to remove autostart file: `" EXECUTABLE_NAME ".desktop`.");
			return false;
		}
		return true;
	}

	if (!file.open(QFile::WriteOnly)) {
		qWarning() << "Unable to open autostart file: `" EXECUTABLE_NAME ".desktop`.";
		return false;
	}

	QString exec = getApplicationPath();

	QDir dir;
	QString iconPath;
	bool haveIcon = false;
	if (!dir.mkpath(IconsDirectory)) // Scalable icons folder may be created
		qWarning() << "Cannot create scalable icon path at " << IconsDirectory;
	else {
		iconPath = IconsDirectory + EXECUTABLE_NAME + ".svg";
		QFile icon(Constants::WindowIconPath);
		if (!QFile(iconPath).exists()) { // Keep old icon but copy if it doesn't exist
			haveIcon = icon.copy(iconPath);
			if (!haveIcon) qWarning() << "Couldn't copy icon svg into " << iconPath;
			else { // Update permissions
				QFile icon(iconPath);
				icon.setPermissions(icon.permissions() | QFileDevice::WriteOwner);
			}
		} else {
			qInfo() << "Icon already exists in " << IconsDirectory << ". It is not replaced.";
			haveIcon = true;
		}
	}

	QTextStream(&file) << QString("[Desktop Entry]\n"
	                              "Name=" APPLICATION_NAME "\n"
	                              "GenericName=SIP Phone\n"
	                              "Comment=" APPLICATION_DESCRIPTION "\n"
	                              "Type=Application\n")
	                   << (openInBackground ? "Exec=" + exec + " --iconified %u\n" : "Exec=" + exec + " %u\n")
	                   << (haveIcon ? "Icon=" + iconPath + "\n" : "Icon=" EXECUTABLE_NAME "\n")
	                   << "Terminal=false\n"
	                      "Categories=Network;Telephony;\n"
	                      "MimeType=x-scheme-handler/sip-" EXECUTABLE_NAME ";x-scheme-handler/sips-" EXECUTABLE_NAME
	                      ";x-scheme-handler/" EXECUTABLE_NAME "-sip;x-scheme-handler/" EXECUTABLE_NAME
	                      "-sips;x-scheme-handler/sip;x-scheme-handler/sips;x-scheme-handler/tel;x-scheme-handler/"
	                      "callto;x-scheme-handler/" EXECUTABLE_NAME "-config;\n"
	                      "X-PulseAudio-Properties=media.role=phone\n";

	return true;
}
#elif defined(Q_OS_MACOS)
// On MAC, URI handlers call the application with no arguments and pass them in event loop.
bool App::event(QEvent *event) {
	if (event->type() == QEvent::FileOpen) {
		const QString url = static_cast<QFileOpenEvent *>(event)->url().toString();
		if (isSecondary()) {
			sendMessage(url.toLocal8Bit(), -1);
			::exit(EXIT_SUCCESS);
		}
		receivedMessage(0, url.toLocal8Bit());
	} else if (event->type() == QEvent::ApplicationStateChange) {
		auto state = static_cast<QApplicationStateChangeEvent *>(event);
		if (state->applicationState() == Qt::ApplicationActive) Utils::smartShowWindow(getMainWindow());
	}

	return SingleApplication::event(event);
}

#endif