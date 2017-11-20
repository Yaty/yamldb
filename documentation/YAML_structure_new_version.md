# Structure des BDD YAML : Proposition

## Version lourde : actuellement utilisée

### Bases

```
data/
├─ base_1.yml
├─ base_2.yml
├─ base_1/
├─ base_2/
```

Contenu des fichiers base.yml :

``` yaml
tables:
    1: table_name_1
    2: table_name_2
```
### Description des tables

```
data/
├─ base_1.yml
├─ base_2.yml
├─ base_1/
│    ├─ table1.yml
│    ├─ table1/
│    ├─ table2.yml
│    ├─ table2/
│    ├─ ...
├─ base_2/
│    ├─ table1.yml
│    ├─ table1/
│    ├─ table2.yml
│    ├─ table2/
│    ├─ ...
```

Contenu des fichiers table.yml :

``` yaml
structure:
    column_name: Type
    ...
```
``` yaml - table1
structure:
    id: integer
    name: string
    lastname: string
    address: string
    ...
```

### Les données
```
data/
├─ base_1.yml
├─ base_2.yml
├─ base_1/
│    ├─ table1.yml
│    ├─ table1/
│    │    ├─ data.yml
│    ├─ table2.yml
│    ├─ table2/
│    │    ├─ data.yml
│    ├─ ...
├─ base_2/
│    ├─ table1.yml
│    ├─ table1/
│    │    ├─ data.yml
│    ├─ table2.yml
│    ├─ table2/
│    │    ├─ data.yml
│    ├─ ...
```

Contenu de data.yml :

``` yaml
data:
    - id: 1
      name: Michel
      lastname: Dupont
      address: 8 rue de l'église
    - id: 2
      name: Dupont
      lastname: Michel
      address: 5 rue de l'église
```
