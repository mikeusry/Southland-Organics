<?php

/**
 * @file
 * Advanced catalog administration menu items.
 */

/**
 * Implementation of hook_form_FORM_ID_alter().
 */
function uc_advanced_catalog_form_uc_catalog_settings_form_alter(&$form, &$form_state) {
  // no more product nodes per page, used on limit filter
  unset($form['catalog-products-list']);
}

/**
 * Advanced catalog settings form.
 *
 * Configure the display of the catalog pager and filters.
 *
 * @ingroup forms
 */
function uc_advanced_catalog_settings_form($form) {
  drupal_add_css(drupal_get_path('module', 'uc_advanced_catalog') .'/uc_advanced_catalog.css');
  $form = array();
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#collapsed' => FALSE,
    '#collapsible' => TRUE,
    '#title' => t('Advanced catalog filters'),
    '#description' => t('The checked filters will be included in your catalog filter area.') .'<br/>'.
    t('Select which default view will be used when available, even if filter is not displayed.') .'<br/>'.
    t('You can reorder filters, first one will be on the left side.'),
  );

  // Sortable table
  $form['filters']['list']['#tree'] = TRUE;
  $form['filters']['list']['#theme'] = 'uc_advanced_catalog_settings_filters';

  // get filters
  $filters = uc_advanced_catalog_catalog_filters();
  foreach ($filters AS $values) {
    $filter[] = _uc_advanced_catalog_filter_edit($values);
  }
  // reorder filter by weight
  usort($filter, '_uc_advanced_catalog_compare');
  $form['filters']['list'] += $filter;

  // main filters settings
  $form['filters']['settings'] = array(
    '#type' => 'fieldset',
    '#collapsed' => TRUE,
    '#collapsible' => TRUE,
    '#title' => t('Filters settings'),
  );
  $form['filters']['settings']['uc_advanced_catalog_extra_settings_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Extra information text'),
    '#description' => t('Variables are !start, !end and !total.'),
    '#default_value' => variable_get('uc_advanced_catalog_extra_settings_text', t('Items !start to !end of !total total')),
  );
  $form['filters']['settings']['uc_advanced_catalog_limit_settings_all'] = array(
    '#type' => 'textfield',
    '#title' => t('Limit all filter'),
    '#size' => 5,
    '#maxlenght' => 4,
    '#description' => t('Indicate the maximum limit for <em>all</em> limit selection.'),
    '#default_value' => variable_get('uc_advanced_catalog_limit_settings_all', 100),
    '#element_validate' => array('_uc_advanced_catalog_numeric'),
  );
  $form['filters']['settings']['uc_advanced_catalog_limit_settings_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Limit filter prefix'),
    '#default_value' => variable_get('uc_advanced_catalog_limit_settings_prefix', t('Show')),
  );
  $form['filters']['settings']['uc_advanced_catalog_limit_settings_suffix'] = array(
    '#type' => 'textfield',
    '#title' => t('Limit filter suffix'),
    '#default_value' => variable_get('uc_advanced_catalog_limit_settings_suffix', t('per page')),
  );
  $form['filters']['settings']['uc_advanced_catalog_mode_settings_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('View filter prefix'),
    '#default_value' => variable_get('uc_advanced_catalog_mode_settings_prefix', t('View as')),
  );
  $form['filters']['settings']['uc_advanced_catalog_sort_settings_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Ordering filter prefix'),
    '#default_value' => variable_get('uc_advanced_catalog_sort_settings_prefix', t('Sort by')),
  );
  $form['catalog'] = array(
    '#type' => 'fieldset',
    '#collapsed' => FALSE,
    '#collapsible' => TRUE,
    '#title' => t('Advanced catalog settings'),
  );
  $options = array(
    'top' => t('Top'),
    'bottom' => t('Bottom'),
    'top-bottom' => t('Top and Bottom'),
  );
  $form['catalog']['uc_advanced_catalog_pager'] = array(
    '#type' => 'select',
    '#title' => t('Pager position'),
    '#options' => $options,
    '#description' => t('Select where the pager will be display on catalog.'),
    '#default_value' => variable_get('uc_advanced_catalog_pager', 'top-bottom'),
  );
  $form['catalog']['uc_advanced_catalog_filters'] = array(
    '#type' => 'select',
    '#title' => t('Filters position'),
    '#options' => $options,
    '#description' => t('Select where the filters will be display on catalog.'),
    '#default_value' => variable_get('uc_advanced_catalog_filters', 'top-bottom'),
  );
  // submit handle, need to submit before system_settings to handle op
  $form['#submit'] = array(
    'uc_advanced_catalog_settings_form_submit',
    'system_settings_form_submit',
  );
  return system_settings_form($form);
}

/**
 * Submit handler for the advanced catalog settins form.
 */
function uc_advanced_catalog_settings_form_submit($form, &$form_state) {
  $reset = FALSE;
  if ($form_state['values']['op'] == t('Reset to defaults')) {
    $reset = TRUE;
  }
  // advanced catalog filters variables
  foreach (element_children($form_state['values']['list']) as $key) {
    // weight
    $name = key($form_state['values']['list'][$key]['title']);
    if ($reset) {
      variable_del($name . '_weight');
    }
    else {
      variable_set($name . '_weight', $form_state['values']['list'][$key]['sort']);
    }
    // registered filters
    $filters[$form_state['values']['list'][$key]['name']] = $form_state['values']['list'][$key]['title'][$name];
    // other values
    foreach ($form_state['values']['list'][$key] AS $v) {
      if (is_array($v)) {
        foreach ($v as $variable => $value) {
          if ($reset) {
            variable_del($variable);
          }
          else {
            variable_set($variable, $value);
          }
        }
      }
    }
  }
  // save registered filters for _uc_advanced_catalog_default()
  variable_set('uc_advanced_catalog_filters_registered', $filters);
}

/*
 * Theme the re-ordering form
 *
 * @ingroup themeable
 */
function theme_uc_advanced_catalog_settings_filters($form) {
  drupal_add_tabledrag('advanced-catalog-sort', 'order', 'sibling', 'sort');
  $header = array('', t('Filters'), t('Default value'), t('Options'), '');
  $rows = array();
  foreach (element_children($form) as $key) {
    // Add class to group weight fields for drag and drop.
    $form[$key]['sort']['#attributes']['class'] = 'sort';
    $row = array();
    $row[] = drupal_render($form[$key]['name']);
    $row[] = drupal_render($form[$key]['title']);
    $row[] = drupal_render($form[$key]['default']);
    $row[] = drupal_render($form[$key]['options']);
    $row[] = drupal_render($form[$key]['sort']);
    $rows[$key] = array('data' => $row, 'class' => 'draggable');
  }
  // order by weight
  ksort($rows);
  $output = theme('table', $header, $rows, array('id' => 'advanced-catalog-sort'));
  $output .= drupal_render($form);
  return $output;
}

/**
 * Advanced catalog list settings form.
 *
 * Give information to handle fields and product display
 *
 * @ingroup forms
 */
function uc_advanced_catalog_list_settings_form($form) {
  $form = array();
  $form['fields'] = array(
    '#value' => t('Field activation, position and option is now handle by Display suite and Node display, please visit <a href="!url">Display suite Layout settings</a>, select Catalog list.', array('!url' => url('admin/build/ds/layout'))),
  );
  if (module_exists('imagecache')) {
      $imagecache_presets = imagecache_presets();
      $options = array();
      foreach ($imagecache_presets AS $preset) {
        $options[$preset['presetname']] = $preset['presetname'];
      }
    $form['uc_advanced_catalog_list_imagecache'] = array(
      '#type' => 'select',
      '#title' => t('Image cache preset of single image mode'),
      '#options' => $options,
      '#description' => t('Choose the preset that will be used for the image field.'),
      '#default_value' => variable_get('uc_advanced_catalog_list_imagecache', 'product_list'),
    );
  }
  return system_settings_form($form);
}

/**
 * Advanced catalog table settings form.
 *
 * Settings for the table display
 * @todo add more settings with field and order selection
 *
 * @ingroup forms
 */
function uc_advanced_catalog_table_settings_form($form) {
  $form = array();
  $form['uc_advanced_catalog_table_header'] = array(
    '#type' => 'select',
    '#title' => t('Header display'),
    '#options' => drupal_map_assoc(array(t('Hide'), t('Show'))),
    '#description' => t('Select display of the table header row. Note: header will not be sortable.'),
    '#default_value' => variable_get('uc_advanced_catalog_table_header', t('Hide')),
  );
  return system_settings_form($form);
}

/**
 * Implementation of hook_form_FORM_ID_alter().
 */
function uc_advanced_catalog_form_uc_product_field_settings_form_alter(&$form, &$form_state) {
  $form['uc_advanced_catalog_infos'] = array(
    '#value' => t('Advanced catalog notice: If you need more fined settings on node full display, fields activation, position and option can be handle by Display suite, please visit <a href="!url">Display suite Layout settings</a>. On full node settings.', array('!url' => url('admin/build/ds/layout'))) .'<br/>',
    '#weight' => 0,
  );
}

/**
 * Implementation of hook_form_FORM_ID_alter().
 *
 */
function uc_advanced_catalog_form_uc_catalog_grid_settings_form_alter(&$form, &$form_state) {
  // no more grid setting
  unset($form['uc_catalog_grid_display']);
  // no more fields selection (handle by Display suite and Node display)
  unset($form['displayed_fields']);
  $form['uc_advanced_catalog_grid_fill'] = array(
    '#type' => 'checkbox',
    '#title' => t('Fill up single line'),
    '#description' => t('If you disable this option a grid with only one row will have the amount of items as tds. If you disable it this can cause problems with your css.'),
    '#default_value' => variable_get('uc_advanced_catalog_grid_fill', TRUE),
    '#weight' => 0.0001,
  );
  if (module_exists('imagecache')) {
      $imagecache_presets = imagecache_presets();
      $options = array();
      foreach ($imagecache_presets AS $preset) {
        $options[$preset['presetname']] = $preset['presetname'];
      }
    $form['uc_advanced_catalog_grid_imagecache'] = array(
      '#type' => 'select',
      '#title' => t('Image cache preset of single image mode'),
      '#options' => $options,
      '#description' => t('Choose the preset that will be used for the image field.'),
      '#default_value' => variable_get('uc_advanced_catalog_grid_imagecache', 'product_list'),
      '#weight' => 0.0002,
    );
  }
  // add information
  $form['displayed_fields'] = array(
    '#value' => t('Fields activation, position and option is now handle by Display suite, please visit <a href="!url">Display suite Layout settings</a>, select Catalog Grid.', array('!url' => url('admin/build/ds/layout'))) .'<br/>',
    '#weight' => 0.0003,
  );
}

/**
 * Helper function for sorting rows on settings form
 * Code from sorting blocks on admin/build/block.
 *
 * Sorted by weight, then by title.
 */
function _uc_advanced_catalog_compare($a, $b) {
  // Sort by weight.
  $weight = $a['sort']['#default_value'] - $b['sort']['#default_value'];
  if ($weight) {
    return $weight;
  }
  // Sort by title.
  if (is_array($a['title'])) {
    $a['title'] = _uc_advanced_catalog_retrieve_title($a['title']);
  }
  if (is_array($b['title'])) {
    $b['title'] = _uc_advanced_catalog_retrieve_title($b['title']);
  }
  if (is_string($a['title']) && is_string($b['title'])) {
    return strcmp($a['title'], $b['title']);
  }
  
}
/**
 * Helper function for sorting rows on settings form
 * Code from sorting blocks on admin/build/block.
 *
 * Sorted by weight, then by title.
 */
function _uc_advanced_catalog_retrieve_title($elements) {
  $output = '';
  if (is_array($elements)) {
    foreach ($elements as $element) {
      if (is_string($element['#title'])) {
        $output = $element['#title'];
      }
    }
  }
  return $output;
}

function _uc_advanced_catalog_numeric($element, &$form_state) {
  if (!is_numeric($element['#value'])) {
    form_error($element, t('This field need to be numeric.'));
  }
}
