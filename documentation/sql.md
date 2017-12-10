# SQL Queries

## Basic select

`SELECT customerId, customerName, city FROM customers`

`SELECT * FROM products`

## Multi table select

`SELECT customerId, productId FROM customers, products`

## Joints

### Inner join

`SELECT customerName, orderId FROM customers JOIN orders ON customers.customerId = orders.customerId`

### Left join

`SELECT customerName, orderId FROM customers LEFT JOIN orders ON customers.customerId = orders.customerId`

### Right join

`SELECT customerName, orderId FROM customers RIGHT JOIN orders ON customers.customerId = orders.customerId`

### Full join

`SELECT customerName, orderId FROM customers FULL JOIN orders ON customers.customerId = orders.customerId`

### Multi joins

`SELECT customerName, orderId FROM customers JOIN orders ON customers.customerId = orders.customerId JOIN orderDetails ON orders.orderId = orderDetails.orderId`

### Joins with multiple fields

TODO

## Order by

TODO

## Conditions

TODO