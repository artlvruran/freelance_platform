#!/bin/bash

INPUT=""
OUTPUT=""

while getopts ":i:o:" opt; do
  case $opt in
    i)
      INPUT=$OPTARG
      ;;
    o)
      OUTPUT=$OPTARG
      ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      exit 1
      ;;
    :)
      echo "Option -$OPTARG requires an argument." >&2
      exit 1
      ;;
  esac
done

# копируем файлик конфигурации в папку билда
cp $INPUT/config.json $OUTPUT
cp -R $INPUT/media $OUTPUT
# сюда пишем все шаблоны
TEMPLATES="$INPUT/templates/master.tmpl $INPUT/templates/signup.tmpl $INPUT/templates/projects.tmpl $INPUT/templates/project.tmpl $INPUT/templates/add_project.tmpl"

# прожевываем шаблоны в срр-шник
cppcms_tmpl_cc $TEMPLATES -o $INPUT/all_tmpl.cpp

# собираем шаблоны в библиотеку
g++ -shared -fPIC $INPUT/all_tmpl.cpp -o $OUTPUT/libcpp_defskin.so -lcppcms -lbooster