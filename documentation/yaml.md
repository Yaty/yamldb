# Structure des BDD YAML : Proposition

## Version lourde

### Bases

```
data/
├─ bases.yaml
├─ base1/
│    ├─ ...
├─ base2/
│    ├─ ...
```

Contenu de bases.yaml :

``` yaml
base1:
    name: nomBase
    description: blabla
base2:
    ...
```

### Description de la base : tables.yaml

```
data/
├─ bases.yaml
├─ base1/
│    ├─ tables.yaml
│    ├─ tables/
│    |    ├─ table1/
│    |    ├─ table2/
```

Contenu de tables.yaml :

``` yaml
table1:
    name: nomTable
    description: blabla
table2:
    ...
```

### Description d'une table

```
data/
├─ bases.yaml
├─ base1/
│    ├─ tables.yaml
│    ├─ tables/
│    |    ├─ table1/
│    |    |    ├─ metadata.yaml
```

Contenu de metadata.yaml :

``` yaml
name: nomTable
description: blabla
columns:
    id: integer
    name: string
    lastname: string
    address: string
    ...
```

### Stockage d'une table

```
data/
├─ bases.yaml
├─ base1/
│    ├─ tables.yaml
│    ├─ tables/
│    |    ├─ table1/
│    |    |    ├─ metadata.yaml
│    |    |    ├─ data.yaml
```

Contenu de data.yaml :

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

## Version allégée

### Bases

```
data/
├─ base1/
│    ├─ ...
├─ base2/
│    ├─ ...
```

### Tables

```
data/
├─ base1/
│    ├─ table1/
│    ├─ table2/
```

### Stockage et description d'une table

```
data/
├─ base1/
│    ├─ table1/
│    |    ├─ data.yaml
```

Contenu de data.yaml :

``` yaml
metadatas:
    name: nomTable
    description: blabla
    columns:
        id: integer
        name: string
        lastname: string
        address: string
        ...
data:
    - id: 1
      name: Michel
      lastname: Dupont
      address: 8 rue de l'église
    - id: 2
      name: Dupont
      lastname: Michel
      address: 5 rue de l'église
    - ...
```