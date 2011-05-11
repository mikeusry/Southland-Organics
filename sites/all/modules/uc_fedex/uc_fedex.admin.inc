<?php
// $Id: uc_fedex.admin.inc,v 1.2 2009/06/07 18:21:37 tr Exp $

/**
 * @file
 * FedEx Shipping Quotes module administration menu and callbacks
 *
 * @author Tim Rohaly.
 * @version $Id: uc_fedex.admin.inc,v 1.2 2009/06/07 18:21:37 tr Exp $
 */


/**
 * Default FedEx Web Services API settings.
 *
 * Records FedEx account information neccessary to use service. Allows testing
 * or production mode. Configures which FedEx services are quoted to customers.
 *
 * @return
 *   Forms for store administrator to set configuration options.
 */
function uc_fedex_admin_settings() {

  // Container for credentials forms
  $form['uc_fedex_credentials'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Credentials'),
    '#description'  => t('Account number and authorization information'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
  );

  // Form to set the developer key
  $form['uc_fedex_credentials']['uc_fedex_user_credential_key'] = array(
    '#type'          => 'textfield',
    '#title'         => t('FedEx Web Services API User Key'),
    '#default_value' => variable_get('uc_fedex_user_credential_key', ''),
    '#required'      => TRUE,
  );

  // Form to set the developer password
  $form['uc_fedex_credentials']['uc_fedex_user_credential_password'] = array(
    '#type'          => 'password',
    '#title'         => t('FedEx Web Services API Password'),
    '#default_value' => variable_get('uc_fedex_user_credential_password', ''),
  );

  // Form to set user account number
  $form['uc_fedex_credentials']['uc_fedex_account_number'] = array(
    '#type'          => 'textfield',
    '#title'         => t('FedEx Account #'),
    '#default_value' => variable_get('uc_fedex_account_number', 0),
    '#required'      => TRUE,
  );

  // Form to set user meter number
  $form['uc_fedex_credentials']['uc_fedex_meter_number'] = array(
    '#type'          => 'textfield',
    '#title'         => t('FedEx Meter #'),
    '#default_value' => variable_get('uc_fedex_meter_number', 0),
    '#required'      => TRUE,
  );

  // Form to set choose between Production and Testing server
  // ***Defaults to Testing!***
  $form['uc_fedex_credentials']['uc_fedex_server_role'] = array(
    '#type'        => 'select',
    '#title'       => t('FedEx Server Role'),
    '#description' => t('Quotes and shipments requested in Testing mode will not be picked up or charged to your account.'),
    '#options'     => array(
      'testing'    => t('Testing'),
      'production' => t('Production'),
    ),
    '#default_value' => variable_get('uc_fedex_server_role', 'testing'),
  );

  // Form to set choose between LIST quotes and ACCOUNT quotes
  // ***Defaults to LIST!***
  $form['uc_fedex_quote_type'] = array(
    '#type'        => 'select',
    '#title'       => t('FedEx Quote Type'),
    '#description' => t('Choose to present the customer with FedEx list prices or your discounted FedEx account prices. LIST prices only exist for US shipments - if you specify LIST for international shipments you will not receive any quotes.  Note that ACCOUNT prices are accurate only on the PRODUCTION server!'),
    '#options'     => array(
      'list'    => t('List Prices'),
      'account' => t('Discount Account Prices'),
    ),
    '#default_value' => variable_get('uc_fedex_quote_type', 'list'),
  );

  // Form to restrict FedEx services available to customer
  $form['uc_fedex_services'] = array(
    '#type'          => 'checkboxes',
    '#title'         => t('FedEx Services'),
    '#default_value' => variable_get('uc_fedex_services', _uc_fedex_services()),
    '#options'       => _uc_fedex_services(),
    '#description'   => t('Select the FedEx services customers are allowed to use.'),
  );

  // Form to set how the package is handed over to FedEx
  $form['uc_fedex_dropoff_type'] = array(
    '#type'          => 'select',
    '#title'         => t('FedEx Pickup/Dropoff Options'),
    '#default_value' => variable_get('uc_fedex_dropoff_type', _uc_fedex_dropoff_types()),
    '#options'       => _uc_fedex_dropoff_types(),
    '#description'   => t('Pickup/Dropoff options.  It is assumed that all your packages are using the same method.'),
  );

  // Form to select FedEx packaging to use
  $form['uc_fedex_package_type'] = array(
    '#type'          => 'select',
    '#title'         => t('FedEx Package Type'),
    '#default_value' => variable_get('uc_fedex_package_type', _uc_fedex_package_types()),
    '#options'       => _uc_fedex_package_types(),
    '#description'   => t('Package Type.  It is assumed that all your packages are using the same packaging.'),
  );

  // Form to select Residential/Commercial destination address
  $form['uc_fedex_residential_quotes'] = array(
    '#type'          => 'radios',
    '#title'         => t('Quote rates assuming destination is a'),
    '#default_value' => variable_get('uc_fedex_residential_quotes', 1),
    '#options'       => array(
      0 => t('Commercial address'),
      1 => t('Residential address (extra fees)'),
    ),
  );

  // Form to add optional Insurance coverage in the amount of the order
  $form['uc_fedex_insurance'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Add Insurance to shipping quote'),
    '#default_value' => variable_get('uc_fedex_insurance', FALSE),
    '#description'   => t('When enabled, products are insured for their full value.'),
  );

  // Container for markup forms
  $form['uc_fedex_markups'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Markups'),
    '#description'  => t('Modifiers to the shipping weight and quoted rate'),
    '#collapsible'  => TRUE,
    '#collapsed'    => FALSE,
  );

  // Form to select type of rate markup
  $form['uc_fedex_markups']['uc_fedex_rate_markup_type'] = array(
    '#type'          => 'select',
    '#title'         => t('Rate Markup Type'),
    '#default_value' => variable_get('uc_fedex_rate_markup_type', 'percentage'),
    '#options'       => array(
      'percentage' => t('Percentage (%)'),
      'multiplier' => t('Multiplier (×)'),
      'currency'   => t('Addition (!currency)', array('!currency' => variable_get('uc_currency_sign', '$'))),
    ),
  );

  // Form to select type of rate amount
  $form['uc_fedex_markups']['uc_fedex_rate_markup'] = array(
    '#type'          => 'textfield',
    '#title'         => t('FedEx Shipping Rate Markup'),
    '#default_value' => variable_get('uc_fedex_rate_markup', '0'),
    '#description'   => t('Markup FedEx shipping rate quote by dollar amount, percentage, or multiplier.'),
  );

  // Form to select type of weight markup
  $form['uc_fedex_markups']['uc_fedex_weight_markup_type'] = array(
    '#type'          => 'select',
    '#title'         => t('Weight Markup Type'),
    '#default_value' => variable_get('uc_fedex_weight_markup_type', 'percentage'),
    '#options'       => array(
      'percentage' => t('Percentage (%)'),
      'multiplier' => t('Multiplier (×)'),
      'mass'   => t('Addition (!mass)', array('!mass' => '#')),
    ),
  );

  // Form to select type of weight markup amount
  $form['uc_fedex_markups']['uc_fedex_weight_markup'] = array(
    '#type'          => 'textfield',
    '#title'         => t('FedEx Shipping Weight Markup'),
    '#default_value' => variable_get('uc_fedex_weight_markup', '0'),
    '#description'   => t('Markup FedEx shipping weight before quote by weight amount, percentage, or multiplier.'),
  );

  // Form to select packaging type
  $form['uc_fedex_all_in_one'] = array(
    '#type'          => 'radios',
    '#title'         => t('Number of Packages'),
    '#default_value' => variable_get('uc_fedex_all_in_one', 1),
    '#options' => array(
      0 => t('Each product in its own package'),
      1 => t('All products in one package'),
    ),
    '#description' => t('Indicate whether each product is quoted as shipping separately or all in one package.'),
  );

  // Register validation handler
  $form['#validate'][] = 'uc_fedex_admin_settings_validate';

  return system_settings_form($form);
}


/**
 * Validation handler for uc_fedex_admin_settings form.
 *
 * Require password only if it hasn't been set.
 *
 */
function uc_fedex_admin_settings_validate($form, &$form_state) {

  $old_password = variable_get('uc_fedex_user_credential_password', '');
  if (!$form_state['values']['uc_fedex_user_credential_password']) {
    if ($old_password) {
      form_set_value($form['uc_fedex_credentials']['uc_fedex_user_credential_password'], $old_password, $form_state);
    }
    else {
      form_set_error('uc_fedex_user_credential_password', t('Password field is required.'));
    }
  }
}
