# Реализация муравьиного алгоритма

### Как собрать проект?

```
cd AntColonyOptimization
mkdir build && cd build
cmake ..
cmake --build . --target aco_project
```

### Как запустить проект?
```
./aco_project ../examples/warehouse.csv
```

### Как запустить тесты?

Если у вас установлена библиотека [Сatch2](https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md#installing-catch2-from-git-repository), то можно собрать и запустить тесты.

```
cmake --build . --target tests
./tests/tests
```