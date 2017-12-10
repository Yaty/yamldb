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

`SELECT customerName, orderId FROM customers JOIN orders ON customers.customerId = orders.customerId JOIN orderDetails ON orders.orderId = orderDetails.orderId`

### Joins with multiple fields

TODO

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

TODO

## Known limitations

We can't use different join type in a query. It won't crash but the result will not be correct.