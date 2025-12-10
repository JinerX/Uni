By skompilować kod należy mieć zainstalowaną bibliotekę `cxxopts`

``` bash
sudo apt update
sudo apt install libcxxopts-dev
```

następnie starczy użyć komendy `make`

przykład odpalenia:
`./SHORTEST_PATH -d <graph.gr> -ss <start_points.ss> -oss <output.ss.res>`
