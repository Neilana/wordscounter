#pragma once

#include <QObject>
#include <QPointF>
#include <QCursor>

// https://stackoverflow.com/questions/39088835/dragging-frameless-window-jiggles-in-qml

class CursorPosProvider : public QObject {
  Q_OBJECT
public:
  explicit CursorPosProvider(QObject *parent = nullptr) : QObject(parent) {}
  virtual ~CursorPosProvider() = default;

  Q_INVOKABLE QPointF cursorPos() { return QCursor::pos(); }
};
