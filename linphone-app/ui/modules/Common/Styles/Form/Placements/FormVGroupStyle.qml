pragma Singleton
import QtQml 2.2

import Units 1.0
import ColorsList 1.0

// =============================================================================

QtObject {
  property int spacing: 5

  property QtObject content: QtObject {
    property int maxWidth: 400
  }

  property QtObject error: QtObject {
    property color color: ColorsList.add("FormV_error", "error").color
    property int pointSize: Units.dp * 10
    property int height: 11
  }

  property QtObject legend: QtObject {
    property color color: ColorsList.add("FormV_legend", "j").color
    property int pointSize: Units.dp * 10
  }
}
