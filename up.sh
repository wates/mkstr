#!/bin/sh
rm build.zip
cp ubuntu/mkstr lambda/mkstr
cd lambda
zip ../build.zip index.js mkstr
cd ..
aws lambda update-function-code --function-name make_string --zip-file fileb://./build.zip
