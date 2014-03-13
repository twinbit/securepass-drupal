<?php

use Securepass\Securepass;
use Securepass\Exception\Exception as SecurepassException;

/**
 * Check if API keys are correctly configured.
 *
 * @param boolean $verbose Print debug message
 */
function _securepass_check_api($verbose = false) {
  $app_id = variable_get('securepass_app_id', FALSE);
  $app_secret = variable_get('securepass_app_secret', FALSE);
  if ($app_id && $app_secret) {
    $config = array('app_id'     => $app_id,
                    'app_secret' => $app_secret);
    return $config;
  }
  if ($verbose) {
    drupal_set_message(t('Securepass API Keys are not configured, you can <a href="@url"> configure api keys by clicking here</a>',
                       array('@url' => url('admin/config/system/securepass/api-keys'))), 'warning');
  }
}

/**
 * Get Securepass client
 */
function securepass_client() {
  if ($config = _securepass_check_api()) {
    try {
      $securepass = new Securepass($config);
      return $securepass;
    }
    catch (SecurepassException $e) {
      $error_message = $e->getMessage();
      drupal_set_message('Securepass: ' . $error_message, 'error');
      watchdog('securepass', 'Securepass error: @error', array('@error' => $error_message), WATCHDOG_ERROR);
    }
  }
}

/**
 * Ping Securepass API
 */
function securepass_api_check_status($verbose = false) {
  try {
    $client = securepass_client();
    $client->ping();
    return true;
  }
  catch (SecurepassException $e) {
    $error = $e->getMessage();
    if ($verbose) {
      drupal_set_message('Securepass: ' . $error, 'error');
    }
    return $error;
  }
}