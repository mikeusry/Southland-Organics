<?php
// $Id: audiofield_formatter.inc,v 1.4 2010/07/12 15:35:48 tamerzg Exp $:
/**@file
 * Theme function for 'embedded' audio.
 */
function theme_audiofield_formatter_audiofield_embedded($element) {
global $base_path;

//Don't render player if there is no file.
  if (!$element['#item']['filepath']) {
    return '';
  }

$player=variable_get('audiofield_audioplayer', 0);

  if ($player==0) {
    $output='<object id="audioplayer2" height="24" width="290" data="' . $base_path . 'sites/all/libraries/player/audio-player/player.swf" type="application/x-shockwave-flash">
             <param value="' . $base_path . 'sites/all/libraries/player/audio-player/player.swf" name="movie"/>
             <param value="playerID=2&amp;bg=0xCDDFF3&amp;leftbg=0x357DCE&amp;lefticon=0xF2F2F2&amp;rightbg=0xF06A51&amp;rightbghover=0xAF2910&amp;righticon=0xF2F2F2&amp;righticonhover=0xFFFFFF&amp;text=0x357DCE&amp;slider=0x357DCE&amp;track=0xFFFFFF&amp;border=0xFFFFFF&amp;loader=0xAF2910&amp;soundFile=' . $base_path . $element['#item']['filepath'] . '" name="FlashVars"/>
             <param value="high" name="quality"/>
             <param value="false" name="menu"/>
             <param value="#FFFFFF" name="bgcolor"/></object>';
  }
  elseif ($player==1) {
     $output='<object type="application/x-shockwave-flash" width="200" height="170"
              data="' . $base_path . 'sites/all/libraries/player/xspf_player_slim.swf?song_url=' . $base_path . $element['#item']['filepath'] . '">
             <param name="movie" value="' . $base_path . 'sites/all/libraries/player/xspf_player_slim.swf?song_url=' . $base_path . $element['#item']['filepath'] . '" />
             </object>';
  }
  elseif ($player==2) {
   $output ='<object><param name="autoplay" value="true" />
              <param name="controller"value="true" /> 
              <embed src="' . $base_path . 'sites/all/libraries/player/playerSinglePackage/playerSingle.swf"  width="175" height="80" float="left" wmode="transparent" 
               flashvars="soundPath=' . $base_path . $element['#item']['filepath'] . '" autostart="true" loop="false"  controller="true" bgcolor="#FF9900" pluginspage="http://www.macromedia.com/go/getflashplayer" >
              </embed></object>';
  }
 
  return $output;
}

/** 
 * Theme function for popup audiofield field formatter. 
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
