#!/bin/sh
ulimit -c unlimited
#ulimit -u unlimited

base_dir="$(cd "$(dirname "$0")";pwd)"

$(cd "${base_dir}")


$(cp -rf src/g3log/*.hpp ../../nautilus.library/g3log/)
if [ $? -eq 0 ]; then
  echo "finish copy src/g3log/*.hpp ../../nautilus.library/g3log/"
else
  echo "failed copy src/g3log/*.hpp ../../nautilus.library/g3log/"
fi


$(cp -rf build/*.a ../../nautilus.library/g3log/lib/)
#$(cp -rf ../build64_debug/md_adapter_shfe/*.a ../reference/nautilus.library/md_adapter_shfe_multi_instrumentID/lib/)
if [ $? -eq 0 ]; then
  echo "finish copy build/*.a ../../nautilus.library/g3log/lib/"
else
  echo "failed copy build/*.a ../../nautilus.library/g3log/lib/"
fi
