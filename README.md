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

Пример вывода:
```
Warehouse map:
G G G G Y G G G G 
G G G G G G G G G 
G G G G G G G G G 
G G G G G G G G G 
G G G G R G G G G 
G G G G G G G G G 
G G G G G G G G G 
G G G G T G G G G 
G G G G R G G G G 
Best route:
G G G G Y G G G G 
G G G G . G G G G 
G G G . . G G G G 
G G G . G G G G G 
G G G . R G G G G 
G G G . . G G G G 
G G G G . G G G G 
G G G G T G G G G 
G G G G R G G G G
```

### Как запустить тесты?

Если у вас установлена библиотека [Сatch2](https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md#installing-catch2-from-git-repository), то можно собрать и запустить тесты.

```
cmake --build . --target tests
./tests/tests
```