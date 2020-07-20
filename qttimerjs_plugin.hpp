#ifndef QTTIMERJS_PLUGIN_HPP
#define QTTIMERJS_PLUGIN_HPP

#include <type_traits>

#include <QJSValue>
#include <QMetaObject>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQmlExtensionPlugin>
#include <QTimer>
#include <QVariant>

class TimerJS;
class Lambda;

class QtTimerJSPlugin : public QQmlExtensionPlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
  QtTimerJSPlugin();
  void initializeEngine(QQmlEngine *engine, const char *uri) override;
  void registerTypes(const char *uri) override;

  TimerJS *timerjs;
};

class Lambda {
public:
  Lambda() = default;
  ~Lambda() = default;

  Lambda(const Lambda &fn) { m_function = fn.m_function; }
  Lambda(Lambda &&fn) { m_function = fn.m_function; };

  template <class Fn, std::enable_if_t<
                          !std::is_same_v<std::decay_t<Fn>, Lambda>, int> = 0>
  Lambda(Fn &&function) {
    m_function.reset(new Callable(std::forward<Fn>(function)));
  }

  Lambda &operator=(Lambda &fn) {
    std::swap(*this, fn);
    return *this;
  };

  Lambda &operator=(const Lambda &fn) {
    Lambda temp(fn);
    std::swap(*this, temp);
    return *this;
  }

  friend void swap(Lambda &l, Lambda &r) {
    using std::swap;
    swap(l.m_function, r.m_function);
  }

  void operator()() {
    if (m_function) {
      m_function->operator()();
    }
  }

  struct Concept {
    virtual ~Concept(){};
    virtual void operator()() = 0;
  };

  template <class Fn> struct Callable : Concept {
    Callable(Fn &&function) : m_fn(std::forward<Fn>(function)) {}
    ~Callable() override {}

    void operator()() override { m_fn(); }
    Fn m_fn;
  };

private:
  std::shared_ptr<Concept> m_function;
};
Q_DECLARE_METATYPE(Lambda);

class TimerJS : public QObject {
  Q_OBJECT
public:
  explicit TimerJS(QObject *parent = nullptr) : QObject(parent) {}
  virtual ~TimerJS() override {}

  inline QVariant setTimeout(const QVariant &handler, int delay,
                             bool singleShot) const;

  Q_INVOKABLE QVariant setTimeout(const QVariant &handler, int delay) const;

  Q_INVOKABLE void clearTimeout(const QVariant &handler) const;

  Q_INVOKABLE QVariant setInterval(const QVariant &handler, int delay) const;

  Q_INVOKABLE void clearInterval(const QVariant &handler) const;
};

#endif // QTTIMERJS_PLUGIN_HPP
