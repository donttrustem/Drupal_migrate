<?php

/*
 * Implement hook_migrate_api().
 */
function perla_migrate_api() {
  $api = array(
    'api' => 2,

    // Give the group a human readable title.
    'groups' => array(
      'vision' => array(
        'title' => t('Perla'),
      ),
    ),

    'migrations' => array(
      'LaPerla' => array(
        'class_name' => 'LaPerlaxmlimport',
        'group_name' => 'vision',
      ),
    ),
  );

  return $api;
}
