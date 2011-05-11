<?php
// $Id: seo_checker.theme.inc,v 1.2.2.5 2009/07/30 13:56:20 miruoss Exp $

/**
 * @file
 * Contains the functions used to theme the outputs of this module.
 *
 * Currently this modules implements two custom form elements. The first
 * is a slider that is used on the configuration page to set the thresholds.
 *
 * The second element is used to display the results of the checks after
 * submitting the node_form.
 */


/**
 * Defines the slider form element and its default values
 */
function seo_checker_elements() {
  $type = array();
  $type['seo_slider'] = array(
    '#input' => TRUE,
    '#steps' => 100,
    '#size' => 3,
  );
  $type['seo_check_results'] = array(
    '#results' => array(),
  );
  return $type;
}

/**
 * Implementation of hook_theme().
 */
function seo_checker_theme() {
  $path = drupal_get_path('module', 'seo_checker');
  return array(
    'seo_slider' => array(
       'template' => 'slider-atleast',
       'path' => "$path/theme",
       'arguments' => array('element' => NULL),
    ),
    'seo_check_results' => array(
      'arguments' => array('element' => NULL),
    )
  );
}

/**
 * Implementation of template_preprocess().
 */
function seo_checker_preprocess_seo_slider(&$args) {
  if (module_exists('jquery_ui')) {
    switch ($args['element']['#slider_type']) {
      case 'at_least':
        $args['template_files'][] = 'slider-atleast';
        break;
      case 'range':
        $args['template_files'][] = 'slider-range';
        break;
    }
  }
  else {
    $args['template_files'][] = 'slider-nojquery';
  }
}

/**
 * By default the check results are themed as a normal form item whose
 * value contains the check results, rendered as a table.
 */
function theme_seo_check_results($element) {
  $output = theme(
    'table',
    array(t('Rule'), t('Message'), t('Achieved'), t('Required'), t('Passed')),
    $element['#results'],
    array()
  );
  $element['#type'] = 'item';
  $element['#value'] = $output;
  $element['#prefix'] = '<div class="seo-check-results">';
  $element['#suffix'] = '</div>';
  return drupal_render($element);
}
