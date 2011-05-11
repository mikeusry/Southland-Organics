<?php
// $Id: seo_checker.admin.inc,v 1.3.2.6 2009/10/18 14:41:47 miruoss Exp $

/**
 * @file
 * The functions used to configure the module over the admin interface.
 *
 */

/**
 * Implementation of hook_perm().
 *
 * @return (array) permissions
 */
function seo_checker_perm() {
    return array('administer seo_checker configuration');
}


/**
 * Builds the settings form for the SEO checker using system_settings_form()
 * @return (array) settings form
 */
function seo_checker_settings() {
  /* load the required js and css files */
  _seo_checker_load_slider();

  /* get the rules and create the fieldset with the sliders */
  $rules = module_invoke_all('register_seo_rules');
  $form = array();

  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['general']['seo_checker_allow_failures'] = array(
    '#type' => 'radios',
    '#title' => t('Check policy and display settings'),
    '#default_value' => variable_get('seo_checker_allow_failures', 'show-preview-only'),
    '#options' => array(
      'show-preview-only' => t("Allow check failures and only show the check results on node previews."),
      'show-always' => t("Allow check failures and also show the results when nodes are saved."),
      'no-failures' => t("Don't allow nodes to be saved as long as they keep failing on some tests."),
    ),
    '#description' => t('Please decide if SEO check failures should be allowed and in what cases check results should be displayed.'),
  );

  $form['thresholds'] = array(
    '#type' => 'fieldset',
    '#title' => t('Thresholds for the SEO rules.'),
    '#collapsible' => TRUE,
    '#description' => t('For the following rules, set the threshold in % where the test should be considered as passed. Depending on the type of the rule, you can either chose 0 or 100 sometimes a value in between. <b>A threshold of 0% disables a test.</b> Results of disabled tests will not be displayed.'),
  );

  foreach ($rules as $rid => $rule) {
    $form['thresholds']['seo_threshold_'. $rid] = array(
      '#type' => 'seo_slider',
      '#slider_type' => $rule['threshold type'],
      '#title' => strip_tags(check_markup($rule['name'])),
      '#default_value' => seo_checker_get_rule_threshold($rule, $rid),
      '#description' => strip_tags(check_markup($rule['description'])),
    );
    if ($rule['threshold type'] == 'bool') {
      $form['thresholds']['seo_threshold_'. $rid]['#steps'] = 1;
    }
  }

  return system_settings_form($form);
}


/**
 * The slider requires some java scripts and css to be loaded
 */
function _seo_checker_load_slider() {
  if (module_exists('jquery_ui')) {
    jquery_ui_add('ui.slider', 'none');
    drupal_add_js(drupal_get_path('module', 'seo_checker') ."/js/slider.js");
  }
  drupal_add_css(drupal_get_path('module', 'seo_checker') ."/css/slider.css");
}