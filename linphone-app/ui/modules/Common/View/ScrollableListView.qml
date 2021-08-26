import QtQuick 2.12	//synchronousDrag
import QtQuick.Controls 2.2

import Common 1.0

// =============================================================================

ListView {
  id: view

  // ---------------------------------------------------------------------------

  ScrollBar.vertical: ForceScrollBar {
    id: vScrollBar

    onPressedChanged: pressed ? view.movementStarted() : view.movementEnded()
	visible:view.contentHeight > view.height
  }

  // ---------------------------------------------------------------------------

  boundsBehavior: Flickable.StopAtBounds
  clip: true
  contentWidth: width - (vScrollBar.visible?vScrollBar.width:0)
  spacing: 0
  synchronousDrag:true
  maximumFlickVelocity:-1
  // ---------------------------------------------------------------------------

  // TODO: Find a solution at this bug =>
  // https://bugreports.qt.io/browse/QTBUG-31573
  // https://bugreports.qt.io/browse/QTBUG-49989
}
