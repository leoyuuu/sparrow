class Person {
  var name
  var gender
  var age
  static var num

  new(n, g, a, s) {
    name = n
    gender = g
    age = a
  }

  sayHello() {
      System.print("My name is " + name)
  }
}