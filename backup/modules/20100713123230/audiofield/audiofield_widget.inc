<?php
// $Id: audiofield_widget.inc,v 1.1 2009/12/29 10:31:33 tamerzg Exp $

/**
 * @file
 * audiofield widget hooks and callbacks.
 */

/**
 * Implementation of CCK's hook_widget_settings($op = 'form').
 */
function audiofield_widget_settings_form($widget) {
  $form = module_invoke('filefield', 'widget_settings', 'form', $widget);

  $form['file_extensions'] = array(
    '#type' => 'textfield',
    '#title' => t('Permitted upload file extensions'),
    '#default_value' => is_string($widget['file_extensions']) ? $widget['file_extensions'] :'mp3',
    '#size' => 64,
    '#description' => t('Extensions a user can upload to this field. Separate extensions with a space and do not include the leading dot. Leaving this blank will allow users to upload a file with any extension.'),
    '#weight' => 1,
  );

  return $form;
}

/**
 * Implementation of CCK's hook_widget_settings($op = 'save').
 */
function audiofield_widget_settings_save($widget) {
  $filefield_settings = module_invoke('filefield', 'widget_settings', 'save', $widget);
    
  return $filefield_settings;
}

/**
 * Element #value_callback function.
 */
function audiofield_widget_value($element, $edit = FALSE) {
  $field = filefield_widget_value($element, $edit);
  
  return $field;
}

/**
 * FormAPI theme function. Theme the output of an image field.
 */
function theme_audiofield_widget($element) {
  
  $z=theme('form_element', $element, $element['#children']);
  
  return $z;
}

function theme_audiofield_widget_preview($item) {
  // Remove the current description so that we get the filename as the link.
  if (isset($item['data']['description'])) {
    unset($item['data']['description']);
  }

  return '<div class="audiofield-file-info">'.
           '<div class="filename">'. theme('audiofield_file', $item) .'</div>'.
           '<div class="filesize">'. format_size($item['filesize']) .'</div>'.
           '<div class="filemime">'. $item['filemime'] .'</div>'.
         '</div>';
}

function theme_audiofield_widget_item($element) {
  return theme('filefield_widget_item', $element);
}

/**
 * Custom theme function for audiofield upload elements.
 *
 * This function allows us to put the "Upload" button immediately after the
 * file upload field by respecting the #field_suffix property.
 */
function theme_audiofield_widget_file($element) {
  $output = '';

  $output .= '<div class="audiofield-upload clear-block">';

  if (isset($element['#field_prefix'])) {
    $output .= $element['#field_prefix'];
  }

  _form_set_class($element, array('form-file'));
  $output .= '<input type="file" name="'. $element['#name'] .'"'. ($element['#attributes'] ? ' '. drupal_attributes($element['#attributes']) : '') .' id="'. $element['#id'] .'" size="'. $element['#size'] ."\" />\n";

  if (isset($element['#field_suffix'])) {
    $output .= $element['#field_suffix'];
  }

  $output .= '</div>';

  return theme('form_element', $element, $output);
}
