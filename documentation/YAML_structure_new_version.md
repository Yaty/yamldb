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
Tables:
    value: table_name_1
    value: table_name_2
```
``` yaml - base_1
Tables:
    value: table1
    value: table2
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
Structure:
    Colum_name: Type
    ...
```
``` yaml - table1
Structure:
    id: integer
    name: string
    lastname: string
    address: string
    ...
```

### Description d'une colonne
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