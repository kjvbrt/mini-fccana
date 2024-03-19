if [ "${0}" != "${BASH_SOURCE}" ]; then
  # Setup fixed pre EDM4hep release of Key4hep stack
  source /cvmfs/sw.hsf.org/key4hep/setup.sh -r 2024-03-10

  # Create two additional environmental variables:
  # KEY4HEP_PODIO
  # KEY4HEP_EDM4HEP
  IFS=':' read -r -a KDIRS <<< "${CMAKE_PREFIX_PATH}"

  for KDIR in "${KDIRS[@]}"; do
    if [[ "${KDIR}" == *"/podio/"* ]]; then
      export KEY4HEP_PODIO="$KDIR"
    fi
    if [[ "${KDIR}" == *"/edm4hep/"* ]]; then
      export KEY4HEP_EDM4HEP="$KDIR"
    fi
  done
else
  echo "----> Error: This script is meant to be sourced!"
fi
