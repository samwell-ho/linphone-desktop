pragma Singleton
import QtQml 2.2

import Units 1.0

// =============================================================================

QtObject {
  property int horizontalSpacing: 8
  property int verticalSpacing: 2

  property QtObject presenceLevel: QtObject {
    property int bottomMargin: 1
    property int size: 16
  }

  property QtObject sipAddress: QtObject {
    property color color: Colors.g.color
    property int pointSize: Units.dp * 10
  }

  property QtObject username: QtObject {
    property color color: Colors.j.color
    property int pointSize: Units.dp * 11
  }
    property QtObject messageCounter: QtObject {
      property int bottomMargin: 4
        property int size: 16
    }
}
