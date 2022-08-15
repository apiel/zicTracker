#!/bin/bash

rm -rf waveedit_download
mkdir -p waveedit_download
cd waveedit_download
wget https://waveeditonline.com/wav-files.zip
unzip wav-files.zip
mv wav-files.zip ../waveedit.zip

for file in *; 
    do echo -n -e ZICW\\x40\\x00\\x00\\x00 >> $file; 
done
mv * ../wavetables/.

cd ..
rm -rf waveedit_download
