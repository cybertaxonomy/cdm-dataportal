<?php

// vim: filetype=php

require ("../CDM_DataPortal/CDM_DataPortal.profile");

/************************************************************
*                           MODULES                         *
************************************************************/
function CDM_DataPortal__Testing_profile_modules() {
  return CDM_DataPortal_profile_modules();
}

/************************************************************
*                           DETAILS                         *
************************************************************/
function CDM_DataPortal_Testing_profile_details() {
  return array (
      'name' => 'CDM DataPortal Testing',
      'description' => 'CDM DataPortal installation profile für testing purposes ',
    );
}

function CDM_DataPortal_Testing_profile_final() {

  CDM_DataPortal_profile_final();
 
/************************************************************
*                           BLOCKS                          *
************************************************************/

  // Taxon tree
  db_query(
      "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
      VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
      'cdm_taxontree', 'cdm_tree', 'garland_EDIT', '1', '-9', 'left', '0', '0', '0', '', ''
  );
  // Search Taxa
  db_query(
      "INSERT INTO {blocks} (module,delta,theme,status,weight,region,custom,throttle,visibility,pages,title)
      VALUES ('%s', '%s', '%s', %d, %d, '%s', %d, %d, %d, '%s', '%s')",
      'cdm_dataportal', '2', 'garland', '1', '-10', 'left', '0', '0', '0', '', ''
  );
  
  return;
}

?>
