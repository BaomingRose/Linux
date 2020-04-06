#!/bin/bash

echo "Enter passWord"
read password

#注：空格，一个不能少
if [ "$password" = "mypassword" ];
then
    echo "welcome"
fi
