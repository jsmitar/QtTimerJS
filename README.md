# QtTimerJS Extension

This extension add support for **JS** timer events to Qt5

# Installation

```bash
  cd QtTimerJS
  mkdir build ; cd build
  qmake-qt5 ..
  make
  sudo make install
```

# Timer events function extensions

The `TimerHandler` is an opaque type, you can use this value for cancel the timer

```ts
function setTimeout(cb: () => any, time: number): TimerHandler;

function setInterval(cb: () => any, time: number): TimerHandler;

function clearTimeout(timer: TimerHandler): void;

function clearInterval(timer: TimerHandler): void;
```

NOTE: `clearInterval` is an synonym of `clearTimeout`

# Example

### HOW TO USE

```qml
  import QtQuick 2.0
  import org.jsmitar.timerjs 1.0

  Item {
    function foo() {
      let timer = setTimeout(_ => console.log('HELLO timer'), 100)
      clearTimeout(timer)
      clearTimeout(timer) // is safe call clearTimeout multiple times
    }

    function bar() {
      let timer = setInterval(_ => {
        console.log('HELLO timer')
        clearInterval(timer) // you can use clearTimeout also
      }, 100)
    }
  }
```

For limitations of Qt if you want to use Timer JS events in `.mjs` modules
you need inject the functions inside of `Qt` global variable

```qml
// file: main.qml

  import QtQuick 2.0
  import org.jsmitar.timerjs 1.0

  Item {
    id: root

    Component.onCompleted: {
      Qt.setInterval = setInterval
      Qt.setTimeout = setTimeout
      Qt.clearTimeout = clearTimeout
      Qt.clearInterval = clearInterval
    }

    ...
  }
```

```qml
// file: my_module.mjs

  export function foo() {
    Qt.setInterval(() => console.log('HELLO timer))
  }
```
