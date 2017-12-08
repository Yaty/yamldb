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
    - name : table_name_1
      empty : no
    - name : table_name_2
      empty : yes
```
empty définit si la table contient des enregistrements ou non

### Description des tables

```
data/
├─ base_1.yml
├─ base_2.yml
├─ base_1/
│    ├─ table1/
│    ├─ table2/
│    ├─ ...
├─ base_2/
│    ├─ table1/
│    ├─ table2/
│    ├─ ...
```
Chaque base contient 0 à n tables. Les tables sont définies par un répertoire dans l'arborescence.

### Les données
Arborescence de stockage
```
data/
├─ base_1.yml
├─ base_2.yml
├─ base_1/
│    ├─ table1/
│    │    ├─ metadata.yml
│    │    ├─ data.yml
│    ├─ table2/
│    │    ├─ metadata.yml
│    │    ├─ data.yml
│    ├─ ...
├─ base_2/
│    ├─ table1/
│    │    ├─ metadata.yml
│    │    ├─ data.yml
│    ├─ table2/
│    │    ├─ metadata.yml
│    │    ├─ data.yml
│    ├─ ...
```

Contenu des fichiers metadata.yml :

``` yaml
structure:
    column_name:
        type: type
        primary: no
        defaultValue: no
        autoIncrement: no
    ...
```
``` yaml - table1
structure:
    id:
        type: INT
        primary: no
        defaultValue: no
        autoIncrement: yes
    name:
        type: VARCHAR
        primary: yes
        defaultValue: VALUE
        autoIncrement: no
    ...
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
