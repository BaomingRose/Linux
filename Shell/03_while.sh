#!/bin/bash

sum=0
number=1

while test $number -le 100
do
    sum=$[ $sum + $number ]
    let number=$number+1
done

echo "The summary is $sum"
