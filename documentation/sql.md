# SQL Queries

## Basic select

`SELECT customerId, customerName, city FROM customers`

`SELECT * FROM products` : you can select all columns with *

## Multi table select

`SELECT customerId, productId FROM customers, products`

## Joins

### Inner join

`SELECT customerName, orderId FROM customers JOIN orders ON customers.customerId = orders.customerId`

### Left join

`SELECT customerName, orderId FROM customers LEFT JOIN orders ON customers.customerId = orders.customerId`

### Right join

Not implemented.

### Full join

Not implemented.

### Multi joins

`SELECT * FROM customers JOIN orders ON customers.customerId = orders.customerId JOIN orderDetails ON orders.orderId = orderDetails.orderId JOIN products ON orderDetails.productId = products.productId`

### Joins with multiple fields (conditions)

Without (we will get multiple lines for each customer if there is multiple time this city in the world):
`SELECT * FROM customers JOIN cities ON customers.city = cities.cityName`

To avoid that we could do this :
`SELECT * FROM customers JOIN cities ON customers.city = cities.cityName AND customers.country = cities.cityCountry`

### Logical operators

We handle those operators :
1. AND
2. OR
3. NOT

TODO :
1. ANY
2. BETWEEN
3. EXISTS
4. IN
5. LIKE
6. SOME

### Joins with different conditions

We can handle those comparators :
1. = : equal
2. <> : not equal
3. > : greater
4. >= : greater or equal
5. < : lesser
6. <= : lesser or equal

## Order by

TODO

## Conditions

`SELECT * FROM customers WHERE city = Memphis`

`SELECT * FROM customers WHERE city = Memphis AND country = USA`

`SELECT * FROM customers WHERE city = Memphis OR country = USA`

`SELECT * FROM customers WHERE city = Memphis NOT country = USA`

`SELECT * FROM customers JOIN orders ON customers.customerId = orders.customerId JOIN orderDetails ON orders.orderId = orderDetails.orderId JOIN products ON orderDetails.productId = products.productId WHERE productId = 1`

TODO : Other operators (LIKE, ...)

## Known limitations

We can't use different join type in a query. It won't crash but the result will be incorrect.