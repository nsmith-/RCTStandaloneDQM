#!/bin/bash
# RCT DQM Web Gallery script
# Author: Christos Lazaridis
#
# Expects two arguments: an input directory and a target directory

if [ $# -ne 2 ]
then
  echo ${0} expects exactly two arguments: A source directory and a target directory
  exit 10
fi

if [ ! -d ${1} ]
then
  echo Directory ${1} does not seem to exist!
  exit 5
fi

if [ ! -d ${2} ]
then
  echo Directory ${2} does not seem to exist!
  exit 5
fi

genRunIndex ()
{
  pushd ${1} > /dev/null

  rm -f thumb-*

  RUNNUMBER=`echo ${1} | awk 'BEGIN { FS = "/" } ; { print $NF }'`

  echo Generating plots for Run \#${RUNNUMBER:3:6}

  echo "<html> <head> <title>Regional Calorimeter Trigger Data Quality Monitoring Page</title></head><body> " > index.html
  echo "<h1>RCT DQM Plots<br /> RUN <a href=\"https://cmswbm.web.cern.ch/cmswbm/cmsdb/servlet/RunSummary?RUN=${RUNNUMBER:3:6}\">#${RUNNUMBER:3:6}</a> </h1> " >> index.html

  cat <<EOF >> index.html
<input type="button" onclick="togglePlots()" value="Toggle!" /><br />
<h2 id="label">RCT Digis from CTP7</h2>
<script type="text/javascript">
function togglePlots() {
  ctp7 = document.getElementById('fromCTP7');
  gct  = document.getElementById('fromGCT');
  pum  = document.getElementById('pum');
  label  = document.getElementById('label');
  if ( ctp7.style.display == 'block' ) {
    ctp7.style.display = 'none';
    pum.style.display = 'none';
    gct.style.display = 'block';
    label.innerHTML = 'RCT Digis from GCT';
    label2.innerHTML = 'RCT Digis from GCT';
  } else {
    ctp7.style.display = 'block';
    pum.style.display = 'block';
    gct.style.display = 'none';
    label.innerHTML = 'RCT Digis from CTP7';
    label2.innerHTML = 'RCT Digis from CTP7';
  }
}
</script>
EOF

  echo '<div id="fromCTP7" style="display: block;">' >> index.html
  for img in *_L1TdeRCT.png ; do
    echo Generating thumbnail for $img...
    convert -scale 240 $img thumb-$img
    echo "<a href=\"$img\"><img src=\"thumb-$img\"></a> " >> index.html
  done
  echo '</div>' >> index.html

  echo '<div id="fromGCT" style="display: none;">' >> index.html
  for img in *_L1TdeRCT_FromGCT.png ; do
    echo Generating thumbnail for $img...
    convert -scale 240 $img thumb-$img
    echo "<a href=\"$img\"><img src=\"thumb-$img\"></a> " >> index.html
  done
  echo '</div>' >> index.html
  
  echo '<h2 id="label2">RCT Digis from CTP7</h2>' >> index.html
  echo '<input type="button" onclick="togglePlots()" value="Toggle!" /><br />' >> index.html

  echo '<div id="pum">' >> index.html
  echo '<h2>Pileup Monitoring Plots</h2>' >> index.html
  for img in *_PUM.png ; do
    echo Generating thumbnail for $img...
    convert -scale 240 $img thumb-$img
    echo "<a href=\"$img\"><img src=\"thumb-$img\"></a> " >> index.html
  done
  echo '</div>' >> index.html

  echo "<br />Webpage generated on: " `date ` >> index.html

  echo "</body></html>" >> index.html

  popd > /dev/null
}

genMainIndex()
{
  currDir=`pwd`
  pushd ${1} > /dev/null

  echo "<html> <head> <title>Regional Calorimeter Trigger Data Quality Monitoring Page</title></head><body> " > index.html

  echo "<h1>RCT DQM Plots</h1> " >> index.html

  for dir in run* ; do
    echo Adding ${dir}...
    echo "<a href=\"$dir/index.html\">RCT Run #${dir:3:6} </a>" >> index.html
    cat "$dir/description_L1TdeRCT.txt" >> index.html
    echo "<br />" >> index.html
  done

  echo "<br />Webpage generated on: " `date ` >> index.html

  echo "</body></html>" >> index.html

  popd > /dev/null
}

NEWPLOTS=false
echo Executing...
echo `date`
echo Checking for new runs...

for inputDir in `ls -1 ${1}`
do
  if [[ -d ${1}/${inputDir} ]]
  then
    echo Found ${1}/${inputDir}

    genRunIndex ${1}/${inputDir}

    echo Moving to ${2}
    # rm -rf ${2}/${inputDir}
    mv ${1}/${inputDir} ${2}
    fs setacl ${2}/${inputDir} webserver:afs read

    NEWPLOTS=true
  fi
done

if [ ${NEWPLOTS} == "true" ]
then
  genMainIndex ${2}
  echo Updated main index.
fi

echo Done.
