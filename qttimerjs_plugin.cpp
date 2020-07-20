#include "qttimerjs_plugin.hpp"

#include <qqml.h>

QtTimerJSPlugin::QtTimerJSPlugin() : timerjs(new TimerJS(this)) {}

void QtTimerJSPlugin::initializeEngine(QQmlEngine *engine, const char *) {
  engine->rootContext()->setContextObject(timerjs);
}

void QtTimerJSPlugin::registerTypes(const char *uri) {
  Q_ASSERT(uri == QLatin1String("QtTimerJS"));
  qmlRegisterModule(uri, 1, 0);
}

QVariant TimerJS::setTimeout(const QVariant &handler, int delay,
                             bool singleShot) const {
  auto value = handler.value<QJSValue>();

  if (!value.isCallable()) {
    qWarning() << "handler is not a value callable";
    return {QJSValue::SpecialValue::UndefinedValue};
  }

  QTimer *timer = new QTimer();
  timer->setSingleShot(singleShot);
  timer->callOnTimeout([value]() mutable { value.call(); });

  Lambda disconnect([timer]() mutable {
    if (timer) {
      if (timer->isActive()) {
        QMetaObject::invokeMethod(timer, "stop", Qt::QueuedConnection);
      }
      timer->deleteLater();
      timer = nullptr;
    }
  });
  if (singleShot)
    timer->callOnTimeout(disconnect);

  timer->start(delay + delay * 0.05);

  return QVariant::fromValue(disconnect);
}

QVariant TimerJS::setTimeout(const QVariant &handler, int delay) const {
  return setTimeout(handler, delay, true);
}

void TimerJS::clearTimeout(const QVariant &handler) const {
  if (handler.canConvert<Lambda>()) {
    auto disconnect = handler.value<Lambda>();
    disconnect();
  }
}

QVariant TimerJS::setInterval(const QVariant &handler, int delay) const {
  return setTimeout(handler, delay, false);
}

void TimerJS::clearInterval(const QVariant &handler) const {
  clearTimeout(handler);
}
