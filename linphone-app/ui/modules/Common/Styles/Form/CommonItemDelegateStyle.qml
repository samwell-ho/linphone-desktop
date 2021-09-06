pragma Singleton
import QtQml 2.2

import Units 1.0
import ColorsList 1.0

// =============================================================================

QtObject {
  property QtObject color: QtObject {
    property color hovered: ColorsList.add("CommonItem_hovered", "o").color
    property color normal: ColorsList.add("CommonItem_normal", "q").color
  }

  property QtObject contentItem: QtObject {
    property int iconSize: 20
    property int spacing: 5

    property QtObject text: QtObject {
      property color color: ColorsList.add("CommonItem_text", "d").color
      property int pointSize: Units.dp * 10
    }
  }

  property QtObject indicator: QtObject {
    property color color: ColorsList.add("CommonItem_indicator", "i").color
    property int width: 5
  }

  property QtObject separator: QtObject {
    property color color: ColorsList.add("CommonItem_separator", "c").color
    property int height: 1
  }
}
