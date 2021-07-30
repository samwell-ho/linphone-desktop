pragma Singleton
import QtQml 2.2
import QtQuick 2.7

import Colors 1.0
import Units 1.0

// =============================================================================
QtObject {
	property int height: 500
	property int width: 450
	
	property QtObject mainLayout: QtObject {
		property int topMargin: 15
		property int leftMargin: 25
		property int rightMargin: 25
		property int spacing: 7
		
	}
	property QtObject searchBar : QtObject{
		property int topMargin : 10
	}
	property QtObject results : QtObject{
		property int topMargin : 10
		property color color : Colors.g
		property QtObject title : QtObject{
			property int topMargin: 10
			property int leftMargin: 20
			property color color: Colors.j
			property int pointSize : Units.dp * 11
			property int weight : Font.DemiBold
		}
		property QtObject header: QtObject{
			property int rightMargin: 55
			property color color: Colors.t
			property int weight : Font.Light
			property int pointSize : Units.dp * 10
			
		}
	}	
	
	property QtObject leaveButton : 
	QtObject {
		property QtObject backgroundColor: QtObject {
			property color disabled: Colors.o
			property color hovered: Colors.j
			property color normal: Colors.k
			property color pressed: Colors.i
		}
		
		property QtObject textColor: QtObject {
			property color disabled: Colors.q
			property color hovered: Colors.q
			property color normal: Colors.i
			property color pressed: Colors.q
		}
		property QtObject borderColor : QtObject{
			property color disabled: Colors.q
			property color hovered: Colors.q
			property color normal: Colors.i
			property color pressed: Colors.q
		}
	}
}