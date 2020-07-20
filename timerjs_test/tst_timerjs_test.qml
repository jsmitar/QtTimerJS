import QtQuick 2.0
import QtTest 1.0
import org.jsmitar.timerjs 1.0

TestCase {
  name: "timerjs_test"

  function test_setTimeout() {
    print("START 10 timers")
    const completed = []
    const timers = Array(10)
      .fill(0)
      .map((_, i) => setTimeout(() => completed.push(i), 900))

    wait(1000)
    print("END 10 timers")
    verify(completed.length === 10, 'some timers not yet completed')
  }

  function test_setInterval() {
    print("START interval")
    let counter = 0

    const timer = setInterval(() => {
      if (counter.length >= 100) {
        clearInterval(timer)
      } else {
        counter += 1
      }
    }, 10)

    wait(1100)
    print("END interval")
    verify(counter >= 100, 'counter is less than to 100')
  }

  function test_clearTimeout() {
    print("START 10 timers")
    const timers = Array(10).fill(0)
      .map((_, i) => {
        return setTimeout(() => {
          fail(`${i}: setTimeout completed`)
        }, 1000)
      })

    wait(100)
    timers.forEach(timer => clearTimeout(timer))
    print("STOP 10 timers")
    wait(10)
  }

  function test_clearInterval() {
    print("START 10 intervals")
    const timers = Array(10).fill(0)
      .forEach((_, i) => {
        let n = 0
        const timer = setInterval(() => {
          if (++n > 10) {
            fail(`${i}: setInterval triggered`)
          } else if (n === 10) {
            clearInterval(timer)
            print(`STOP timer: ${i}`)
          }
        }, 50)
      })

    wait(50 * 12)
    print("STOP 10 intervals")
    wait(10)
  }

  function test_precision() {
    print("CALC PRECISION")
    const timers = Array(10).fill(0)
      .forEach((_, i) => {
        const label = `${i}: setInterval ${i * 250}ms`
        console.time(label)
        setInterval(() => console.timeEnd(label), i * 250)
      })

    wait(10 * 250)
  }
}
