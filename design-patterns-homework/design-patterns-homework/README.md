# Java Design Patterns Homework (8-12)

Five standalone Java examples for design-pattern homework. Each example has a `main` method and can be run independently. **Java 17+ required. No external dependencies.**

| # | Pattern | Example | Core idea |
|---|---|---|---|
| 8 | Prototype | Game character templates | Copy an existing object, including mutable nested data |
| 9 | Simple Factory | Email/SMS notifications | Centralize concrete object creation |
| 10 | Abstract Factory | Windows/Mac UI controls | Create matching families of related objects |
| 11 | Adapter | Legacy payment gateway | Convert dollars API to cents API |
| 12 | Composite | Files and folders | Treat a single file and nested folders uniformly |

## Run

From the repository root:

```bash
mkdir -p out
javac -d out src/main/java/com/example/patterns/*.java src/test/java/com/example/patterns/*.java
java -cp out com.example.patterns.PrototypeDemo
java -cp out com.example.patterns.FactoryDemo
java -cp out com.example.patterns.AbstractFactoryDemo
java -cp out com.example.patterns.AdapterDemo
java -cp out com.example.patterns.CompositeDemo
java -ea -cp out com.example.patterns.PatternTests
```

`-ea` enables Java assertions used in the lightweight tests.

## Pattern explanations

### 8. Prototype
`GameCharacter.copy()` constructs an independent copy. The equipment list is mutable, so the copy constructor copies it instead of sharing the original list. Changing the clone does not change the template.

### 9. Simple Factory
`NotificationFactory.create(Channel)` selects the concrete notification class. Client code depends on the `Notification` interface. **Note:** this is Simple Factory, not the GoF Factory Method pattern.

### 10. Abstract Factory
`UiFactory` defines methods for creating both `Button` and `Checkbox`. `WindowsFactory` and `MacFactory` each provide a matching pair. `SettingsScreen` is independent of the concrete UI family.

### 11. Adapter
`Checkout` expects `PaymentProcessor.pay(BigDecimal dollars)`, but `LegacyGateway` accepts cents as a `long`. `LegacyGatewayAdapter` translates calls. `BigDecimal` avoids floating-point rounding mistakes for monetary amounts.

### 12. Composite
`FileNode` is a leaf and `FolderNode` is a composite. Both implement `FileSystemNode`. Folder sizes are calculated recursively. Cycles are rejected to prevent infinite recursion.

## Homework extension ideas

1. Prototype: add an inventory object and ensure it is copied independently.
2. Factory: add a `PUSH` notification channel.
3. Abstract Factory: add a `LinuxFactory` with matching components.
4. Adapter: add a second gateway that accepts an amount in dollars.
5. Composite: add a `countFiles()` operation to files and folders.

## Suggested GitHub repository name
`java-design-patterns-homework`

## Push to GitHub

Create an empty GitHub repository first, then run:

```bash
git init
git add .
git commit -m "Add design patterns 8-12 examples"
git branch -M main
git remote add origin https://github.com/YOUR_USERNAME/java-design-patterns-homework.git
git push -u origin main
```

Replace `YOUR_USERNAME` with your GitHub username. No repository has been created or pushed automatically.
