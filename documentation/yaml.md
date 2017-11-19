# Structure des BDD YAML : Proposition

## Version lourde : actuellement utilisée

### Bases

```
data/
├─ bases.yml
├─ bases/
│    ├─ base1/
│    ├─ base2/
│    ├─ ...
```

Contenu de bases.yml :

``` yaml
base1:
    name: nomBase
    description: blabla
base2:
    ...
```

### Description de la base : tables.yml

```
data/
├─ bases.yml
├─ bases/
│    ├─ base1/
│    │    ├─ tables.yml
│    │    ├─ tables/
│    │    |    ├─ table1/
│    │    |    ├─ table2/
```

Contenu de tables.yml :

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
├─ bases.yml
├─ bases/
│    ├─ base1/
│    │    ├─ tables.yml
│    │    ├─ tables/
│    │    |    ├─ table1/
│    │    |    |    ├─ metadata.yml
```

Contenu de metadata.yml :

``` yaml
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
├─ bases.yml
├─ bases/
│    ├─ base1/
│    │    ├─ tables.yml
│    │    ├─ tables/
│    │    |    ├─ table1/
│    │    |    |    ├─ metadata.yml
│    │    |    |    ├─ data.yml
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