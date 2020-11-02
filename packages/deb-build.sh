#!/bin/bash

# setup git
git config --global user.email "travis@travis-ci.org"
git config --global user.name "Travis CI"

# make the deb file
cp TotalReqall ../packages/deb/bin/totalreqall
dpkg-deb --build ../packages/deb totalreqall.deb

# commit and push
mv totalreqall.deb ../docs/binaries
cd ..
git add .
git commit -m "Deb file for build $TRAVIS_BUILD_NUMBER"
git remote add travis-origin "https://LorenDB:${TRAVIS_DEPLOY_TOKEN}@github.com/LorenDB/TotalReqall.git" 2>&1 > /dev/null
git push travis-origin master