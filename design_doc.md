I'm going to work on a project called Molecular Dynamics Playtest. A key part of this project is to learn how to successfully utilise SOLID principles.

## SOLID Principles

### Single Responsibility Principle

The Single Responsibility Principle (SRP) states that a class should have only one reason to change, meaning it should have only one job or responsibility. This principle helps create more maintainable and focused code by ensuring each class does one thing well.

For example, instead of having a class that handles both user data persistence and email notifications, you should split these into separate classes: one for managing user data and another for handling notifications. This makes the code easier to understand, test, and modify since changes to one responsibility won't affect the other.

When a class has multiple responsibilities, it becomes more difficult to maintain and more prone to bugs, as changes for one responsibility might inadvertently affect another. Following SRP leads to more cohesive, loosely coupled code that's easier to maintain and reuse.


### Open/Closed Principle

The Open/Closed Principle (OCP) states that software entities (classes, modules, functions, etc.) should be open for extension but closed for modification. This means that you should be able to add new functionality without changing existing code.

For example, instead of modifying an existing class to add new behavior, you should be able to extend it through inheritance or interfaces. This helps maintain stability in the codebase and reduces the risk of introducing bugs in existing functionality.


### Liskov Substitution Principle

The Liskov Substitution Principle (LSP) states that objects of a superclass should be replaceable with objects of its subclasses without breaking the application. In other words, what works with a base class should work equally well with any of its derived classes.

This principle helps ensure that inheritance hierarchies are designed correctly and that polymorphism works as intended. If a program is using a base class, it should be able to use any of its derived classes without knowing it's working with a derived class.

For example, if you have a method that works with a Bird class, it should work just as well with a Penguin or Sparrow class that inherits from Bird, without requiring any special cases or type checking. If you find yourself needing to check what type of Bird you're working with, you might be violating LSP.




### Interface Segregation Principle

The Interface Segregation Principle (ISP) states that clients should not be forced to depend on interfaces they don't use. In other words, it's better to have many small, specific interfaces rather than a few large, general-purpose ones.

This principle helps prevent classes from being forced to implement methods they don't need just because those methods are part of a large interface. Instead, interfaces should be broken down into smaller, more focused ones that better match what clients actually need.

For example, if you have a Worker interface with methods for both manual labor and paperwork, it would be better to split it into separate ManualLaborer and PaperworkHandler interfaces. This way, classes that only do manual labor don't need to implement paperwork methods, and vice versa.



### Dependency Inversion Principle

The Dependency Inversion Principle (DIP) states that high-level modules should not depend on low-level modules; both should depend on abstractions. Additionally, abstractions should not depend on details; details should depend on abstractions.

This principle helps create a more flexible and maintainable system by decoupling high-level and low-level components. Instead of having high-level modules directly depend on low-level modules, both should depend on abstract interfaces or abstract classes.

For example, instead of having a high-level ReportGenerator class directly depend on a low-level DatabaseConnection class, both should depend on an abstract DataSource interface. This allows you to easily swap out different data sources (database, file system, web service, etc.) without modifying the ReportGenerator class.


I'm going to use these principles to design a number of systems that work together but are not tightly coupled. These systems will be modules that can be used in future projects. These future projects will be quite different to this one. 

## Systems

### System 1: Visualisation System

### System 2: Interface System

### System 3: Simulation System

### System 4: Data System

### System 5: Contextualisation System

### System 6: Chatbot System

