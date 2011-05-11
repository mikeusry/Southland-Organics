<?php
// $Id: audiofield_formatter.inc,v 1.19 2010/11/15 12:30:54 tamerzg Exp $:
/**@file
 * Theme function for 'embedded' audio.
 */
function theme_audiofield_formatter_audiofield_embedded($element) {
global $user;
//Don't render player if there is no file.
  if (!$element['#item']['filepath']) {
    return '';
  }

  $info = pathinfo($element['#item']['filename']);
  $op = $info['extension'];
  $audiofile=file_create_url($element['#item']['filepath']);
  $output= audiofield_get_player($audiofile,$op);
	
	//If user is file owner check for download permission and display download link
	if($user->uid==$element['#item']['uid'] && user_access('download own audio files')){
	   $output.='<div class="audio-download"><b>Download:</b>'.theme('filefield_file', $element['#item']).'</div>';
    }
	elseif(user_access('download all audio files')){
	//If user has permission to download all audio files, then display download link
		$output.='<div class="audio-download"><b>Download:</b>'.theme('filefield_file', $element['#item']).'</div>';
	}
   
   return $output;
}

/** 
 * TODO:Theme function for popup audiofield field formatter. 
 */
function theme_audiofield_formatter_audiofield_popup($element) {
  
  return $output;
}

/**
 * Theme function for any file that is managed by FileField.
 *
 * It doesn't really format stuff by itself but rather redirects to other
 * formatters that are telling us they want to handle the concerned file.
 *
 * This function checks if the file may be shown and returns an empty string
 * if viewing the file is not allowed for any reason. If you need to display it
 * in any case, please use theme('filefield') instead.
 */
function theme_audiofield_item($file, $field) {
  if (filefield_view_access($field['field_name']) && filefield_file_listed($file, $field)) {
    return theme('filefield_file', $file);
  }
  return '';
}

/**
 * Theme function for the 'generic' single file formatter.
 */
function theme_audiofield_file($file) {
  // Views may call this function with a NULL value, return an empty string.
  if (empty($file['fid'])) {
    return '';
  }

  $path = $file['filepath'];
  $url = file_create_url($path);
  $icon = theme('filefield_icon', $file);

  // Set options as per anchor format described at
  // http://microformats.org/wiki/file-format-examples
  // TODO: Possibly move to until I move to the more complex format described 
  // at http://darrelopry.com/story/microformats-and-media-rfc-if-you-js-or-css
  $options = array(
    'attributes' => array(
      'type' => $file['filemime'] . '; length=' . $file['filesize'],
    ),
  );

  // Use the description as the link text if available.
  if (empty($file['data']['description'])) {
    $link_text = $file['filename'];
  }
  else {
    $link_text = $file['data']['description'];
    $options['attributes']['title'] = $file['filename'];
  }

  return '<div class="filefield-file clear-block">'. $icon . l($link_text, $url, $options) .'</div>';
}