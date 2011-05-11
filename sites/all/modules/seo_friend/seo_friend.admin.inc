<?php

/**
 * @file
 * Admin page callbacks for the seo_friend module.
 */

/**
 * Menu callback for the seo_friend module to display its administration page.
 */
function seo_friend_admin_settings_form() {

  $form = array();

  $type = arg(3) ? arg(3) : 'default';
  if ($type == 'default') {
    $name = 'Default';
    $title = t('SEO Friend Default Settings');
  } else {
    $names = node_get_types('names');
    $name = $names[$type];
    $title = t('SEO Friend @name Settings', array('@name'=>$name));
  }
  $form[$type] = array(
      '#title' => $title,
      '#type' => 'fieldset',
      );

  $key = seo_friend_get_key(SEO_FIELDSETS_COLLAPSED, $type);
  $fieldsets_closed = variable_get($key, TRUE);
  $form[$type][$key] = array(
        '#title' => '<b>'.t('Keep fieldsets below closed?').'</b>',
        '#type' => 'checkbox',
        '#default_value' => $fieldsets_closed,
        '#description' => t('If checked, fieldsets will stay closed when loading
          this page; otherwise, they will stay open.'),
        );

  seo_friend_text_settings($form, $type, $name, 'page_title', $fieldsets_closed);
  seo_friend_text_settings($form, $type, $name, 'nodewords', $fieldsets_closed);
  if ($type == 'default') {
    seo_friend_text_settings($form, $type, $name, 'nodewords_bypath', $fieldsets_closed);
    seo_friend_menu_settings($form, $fieldsets_closed);
    seo_friend_pathauto_settings($form, $fieldsets_closed);
    seo_friend_report_settings($form, $fieldsets_closed);
  }

  return system_settings_form($form);
} // function seo_friend_admin_settings_form

/**
 * Settings for keeping track if the checkbox has been toggled on a node edit
 * form so it can be used later.
 */
function seo_friend_pathauto_settings(&$form, $fieldsets_closed) {
  // check system table to make sure that weights are okay
  $results = db_query("SELECT name, weight FROM {system} WHERE name IN ('seo_friend', 'pathauto')");
  while ($row = db_fetch_object($results)) {
    $weights[$row->name] = $row->weight;
  }
  if (sizeof($weights) == 2) {
    $direction = 'higher';
    $error = '';
    if ($weights['seo_friend'] < $weights['pathauto']) {
      $direction = 'lower';
      $error = '<strong>'.t('UPDATE YOUR SYSTEM TABLE OR USE THE MODULE WEIGHTS MODULE.').'</strong>';
    }
    $weight_msg = t('Currently the seo_friend weight is !direction than the
        pathauto weight. !error', 
        array('!direction' => $direction, '!error' => $error));
  }

  $form['default']['pathauto'] = array(
      '#title' => t('Pathauto Settings'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => $fieldsets_closed,
      '#description' => t('These settings require that the SEO Friend module has
        a higher weight than the Pathauto module. ').$weight_msg,
      );
  if (module_exists('pathauto')) {
    $form['default']['pathauto'][SEO_PATHAUTO_TRACK_STATES] = array(
        '#title' => '<b>'.t('Keep track of pathauto checkbox state?').'</b>',
        '#type' => 'checkbox',
        '#default_value' => variable_get(SEO_PATHAUTO_TRACK_STATES, TRUE),
        '#description' => t('If checked, seo_friend will track if you have
          unchecked the pathauto auto alias checkbox for a particular node so
          that the state of the checkbox will reflect this in subsequent edits.'),
        );
  }
  else {
    $message = seo_friend_module_not_found('pathauto');
    $form['default']['pathauto']['message'] = array(
        '#value' => '<div>' . $message . '</div>',
        );
  }
} // function seo_friend_pathauto_settings

/**
 * Settings for adding nofollow and target=_blank for certain menu items based
 * on path patterns.
 */
function seo_friend_menu_settings(&$form, $fieldsets_closed) {
  $form['default']['menu_attributes'] = array(
      '#title' => t('Menu Item Attribute Settings'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => $fieldsets_closed,
      '#description' => t('Configure which menu items in the system should have special attributes added to them. You will have to pull this into your theme by calling the seo_friend_menu_item_link function from your theme xxxx_menu_item_link function. For greater control, use the !module.', array('!module' => l('Menu Attributes Module', 'http://drupal.org/project/menu_attributes'))),
      );
  $form['default']['menu_attributes'][SEO_NOFOLLOW] = array(
      '#title' => t('Nofollow'),
      '#type' => 'textarea',
      '#default_value' => variable_get(SEO_NOFOLLOW, ''),
      '#description' => t('Add patterns - one per line - for the urls that you want to use nofollow on in the menu system. For example: user/login, *register, admin*, node/1234, foo/bar/blah'),
      );
  $form['default']['menu_attributes'][SEO_BLANK_TARGET] = array(
      '#title' => t('Blank Target'),
      '#type' => 'textarea',
      '#default_value' => variable_get(SEO_BLANK_TARGET, ''),
      '#description' => t('Add patterns - one per line - for the urls that you want to use target=_blank on in the menu system. For example: http://someothersite.com*'),
      );
} // function seo_friend_menu_settings

/**
 * Settings for report settings.
 */
function seo_friend_report_settings(&$form, $fieldsets_closed) {
  $default_max = seo_friend_get_number_variable(SEO_REFERRER_MAX, 10000);
  $default_pager_num = seo_friend_get_number_variable(SEO_PAGER_NUM, 100);
  $form['default']['report'] = array(
      '#title' => t('Report Settings'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => $fieldsets_closed,
      '#description' => t('Configure settings for the SEO reports.'),
      );
  $form['default']['report'][SEO_PAGER_NUM] = array(
      '#title' => t('Report Pager Number'),
      '#type' => 'textfield',
      '#size' => 4,
      '#length' => 4,
      '#default_value' => $default_pager_num,
      '#description' => t('Max number of results to show per report page.'),
      );
  $form['default']['report'][SEO_REFERRER_MAX] = array(
      '#title' => t('Max Referrer Check'),
      '#type' => 'textfield',
      '#size' => 6,
      '#length' => 6,
      '#default_value' => $default_max,
      '#description' => t('The maximum number of items to check in the accesslog
        table for the referrer report. If this number is set too high, then the
        report can run out of memory.'),
      );
} // function seo_friend_report_settings

/**
 * Settings for various SEO text fields such as character length, word length,
 * whether it's required, etc. Relevant for text fields in page_title, nodewords 
 * and nodewords_bypath modules. There are default settings and content type
 * specific settings that can override the defaults.
 */
function seo_friend_text_settings(&$form, $type, $name, $module,
    $fieldsets_closed) {
  if (module_exists($module)) {
    if ($type == 'default') {
      $fieldset_desc = t('These settings will be used for all content types unless you have checked the override checkbox for the specific content type.');
    }
    else {
      $fieldset_desc = t('The default settings will be used for this content type unless you check the override checkbox below.');
      $fieldset_desc2 = t('Only applies if override checkbox is checked.');
    }
  }
  $form[$type][$module] = array(
      '#title' => t('@module @name Settings', array('@module'=>$module, '@name'=>$name)),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => $fieldsets_closed,
      '#description' => $fieldset_desc,
      );
  if (module_exists($module)) {
    $tags = seo_friend_get_tags($module);
    if (! empty($tags)) {
      if (sizeof($tags) > 1) {

        $key = seo_friend_get_key(SEO_FIELDSETS_COLLAPSED_TEXT, $type);
        $fieldsets_closed = variable_get($key, TRUE);
        $form[$type][$module][$key] = array(
            '#title' => '<b>'.t('Keep fieldsets below closed?').'</b>',
            '#type' => 'checkbox',
            '#default_value' => $fieldsets_closed,
            '#description' => t('If checked, fieldsets will stay closed when loading
              this page; otherwise, they will stay open.'),
            );

        $form[$type][$module]['required'] = array(
            '#title' => t('Required'),
            '#type' => 'fieldset',
            '#collapsible' => TRUE,
            '#collapsed' => $fieldsets_closed,
            '#description' => t('Check each field that should be required. ').$fieldset_desc2,
            );
        $form[$type][$module]['min_chars'] = array(
            '#title' => t('Minimum Characters'),
            '#type' => 'fieldset',
            '#collapsible' => TRUE,
            '#collapsed' => $fieldsets_closed,
            '#description' => t('Minimum number of characters. ').$fieldset_desc2,
            );
        $form[$type][$module]['max_chars'] = array(
            '#title' => t('Maximum Characters'),
            '#type' => 'fieldset',
            '#collapsible' => TRUE,
            '#collapsed' => $fieldsets_closed,
            '#description' => t('Maximum number of characters. '.$fieldset_desc2),
            );
        $form[$type][$module]['min_words'] = array(
            '#title' => t('Minimum Words'),
            '#type' => 'fieldset',
            '#collapsible' => TRUE,
            '#collapsed' => $fieldsets_closed,
            '#description' => t('Minimum number of words. ').$fieldset_desc2,
            );
        $form[$type][$module]['max_words'] = array(
            '#title' => t('Maximum Words'),
            '#type' => 'fieldset',
            '#collapsible' => TRUE,
            '#collapsed' => $fieldsets_closed,
            '#description' => t('Maximum number of words. '.$fieldset_desc2),
            );
        $form[$type][$module]['duplicate'] = array(
            '#title' => t('Duplicate Check'),
            '#type' => 'fieldset',
            '#collapsible' => TRUE,
            '#collapsed' => $fieldsets_closed,
            '#description' => t('Check if there are duplicates. '.$fieldset_desc2),
            );
      }
      foreach ($tags as $tag) {
        if ($type != 'default') {
          $override_key = seo_friend_get_key(SEO_OVERRIDE, $module, $type);
          $form[$type][$module][$override_key] = array(
              '#title' => '<b>'.t('Override Default Settings?').'</b>',
              '#type' => 'checkbox',
              '#default_value' => variable_get($override_key, FALSE),
              '#weight' => -10,
              );
        }
        $key = seo_friend_get_key(SEO_REQUIRED, $module, $tag, $type);
        $form[$type][$module]['required'][$key] = array(
            '#title' => seo_friend_get_tag_name($tags, $tag, 'Required?'),
            '#type' => 'checkbox',
            '#default_value' => variable_get($key, FALSE),
            );
        $key = seo_friend_get_key(SEO_MIN_CHARS, $module, $tag, $type);
        $form[$type][$module]['min_chars'][$key] = array(
            '#title' => seo_friend_get_tag_name($tags, $tag, 'Minimum Characters'),
            '#type' => 'textfield',
            '#size' => 4,
            '#maxlength' => 4,
            '#default_value' => variable_get($key, FALSE),
            );
        $key = seo_friend_get_key(SEO_MAX_CHARS, $module, $tag, $type);
        $form[$type][$module]['max_chars'][$key] = array(
            '#title' => seo_friend_get_tag_name($tags, $tag, 'Maximum Characters'),
            '#type' => 'textfield',
            '#size' => 4,
            '#maxlength' => 4,
            '#default_value' => variable_get($key, FALSE),
            );
        $key = seo_friend_get_key(SEO_MIN_WORDS, $module, $tag, $type);
        $form[$type][$module]['min_words'][$key] = array(
            '#title' => seo_friend_get_tag_name($tags, $tag, 'Minimum Words'),
            '#type' => 'textfield',
            '#size' => 4,
            '#maxlength' => 4,
            '#default_value' => variable_get($key, FALSE),
            );
        $key = seo_friend_get_key(SEO_MAX_WORDS, $module, $tag, $type);
        $form[$type][$module]['max_words'][$key] = array(
            '#title' => seo_friend_get_tag_name($tags, $tag, 'Maximum Words'),
            '#type' => 'textfield',
            '#size' => 4,
            '#maxlength' => 4,
            '#default_value' => variable_get($key, FALSE),
            );
        $key = seo_friend_get_key(SEO_DUPLICATES, $module, $tag, $type);
        $form[$type][$module]['duplicate'][$key] = array(
            '#title' => seo_friend_get_tag_name($tags, $tag, 'Duplicate Check'),
            '#type' => 'select',
            '#options' => seo_friend_get_duplicate_select_options(),
            '#default_value' => variable_get($key, seo_friend_get_duplicate_select_default($tag)),
            );
      }
      $form['#validate'][] = 'seo_friend_settings_validate';
    }
  }
  else {
    $message = seo_friend_module_not_found($module);
    $msg = seo_friend_get_key('seo', $module, 'msg');
    $form[$type][$module][$msg] = array(
        '#value' => '<div>' . $message . '</div>',
        );
  }
} // function seo_friend_text_settings

/**
 * Gets a message based if the module is available in the system or not. If not
 * enabled, provides link to modules page; otherwise, provides link to
 * drupal.org project page.
 *
 * @param $module The module to check.
 */
function seo_friend_module_not_found($module) {
  // figure out if module is downloaded but not enabled
  $modules_list = drupal_system_listing('\.module$', 'modules', 'name', 0);
  $message = t('You are not currently using the @module module.', array('@module'=>$module)).' ';
  if (in_array($module, array_keys($modules_list))) {
    $message .= t('You can enable it here: ') . l('admin/build/modules', 'admin/build/modules');
  }
  else {
    $message .= t('You can download it here: ').seo_friend_get_project_page_link($module);
  }
  return $message;
} // function seo_friend_module_not_found

/**
 * Gets the drupal.org project page for the given module. If the module has
 * been marked as 'core', the handbook page is returned. Otherwise, the regular
 * http://drupal.org/project page is returned.
 *
 * @param $module The module to check.
 */
function seo_friend_get_project_page_link($module) {
  $seo_modules = module_invoke_all('seo_modules');
  $module_data = $seo_modules[$module];
  $project = $module_data['project'] ? $module_data['project'] : $module;
  if ($project == 'core') {
    $url = sprintf('http://drupal.org/handbook/modules/%s', $module);
  }
  else {
    $url = sprintf('http://drupal.org/project/%s', $project);
  }
  return l($url, $url, array('attributes' => array('target'=>'_blank')));
} // seo_friend_get_project_page_link

/**
 * Validates the settings chosen such as making sure that a min or max string is
 * a non-zero number.
 */
function seo_friend_settings_validate($form, &$form_state) {
  $type = arg(3) ? arg(3) : 'default';
  $modules = array('nodewords', 'nodewords_bypath', 'page_title');
  foreach ($modules as $module) {
    $tags = seo_friend_get_tags($module);
    if (! empty($tags)) {
      foreach ($tags as $tag) {
        $min_key = seo_friend_get_key(SEO_MIN_CHARS, $module, $tag, $type);
        seo_friend_check_non_zero($form_state, $min_key, 'Minimum chars', $tag);
        $max_key = seo_friend_get_key(SEO_MAX_CHARS, $module, $tag, $type);
        seo_friend_check_non_zero($form_state, $max_key, 'Maximum chars', $tag);
        $min_key = seo_friend_get_key(SEO_MIN_WORDS, $module, $tag, $type);
        seo_friend_check_non_zero($form_state, $min_key, 'Minimum words', $tag);
        $max_key = seo_friend_get_key(SEO_MAX_WORDS, $module, $tag, $type);
        seo_friend_check_non_zero($form_state, $max_key, 'Maximum words', $tag);
      }
    }
  }
} // function seo_friend_settings_validate

/**
 * Checks that the value associated with the given key is numeric, non-zero, and
 * positive.
 *
 * @param $form_state The form_state from the form.
 * @param $key The key for the form field to check.
 * @param $label The text label for the error message.
 * @param $tag The tag associated with the field (e.g. description).
 */
function seo_friend_check_non_zero($form_state, $key, $label, $tag) {
  $value = trim($form_state['values'][$key]);
  if ($value != '') {
    if (! is_numeric($value)) {
      form_set_error($key, t('@label for @tag must be a number', array('@label'=>$label, '@tag'=>$tag)));
    } 
    else if ($value == 0) {
      form_set_error($key, t('@label for @tag must be non-zero', array('@label'=>$label, '@tag'=>$tag)));
    } 
    else if ($value < 0) {
      form_set_error($key, t('@label for @tag must be positive', array('@label'=>$label, '@tag'=>$tag)));
    }
  }
} // function seo_friend_check_non_zero

/**
 * Allow hook for other modules to notify this module of other SEO-related
 * reports not listed here. Hook is hook_seo_report and should return an array
 * of the form:
 *
 * array(
 *   '[report url text]' => array(
 *     '[report title]',
 *     '[report function to call]',
 *    )
 * )
 * e.g.
 * array(
 *   'myseoreport' => array(
 *     'My SEO Report',
 *     'mymodule_my_seo_report',
 *    )
 * )
 *
 */
function seo_friend_reports() {
  $report_list = module_invoke_all('seo_reports');
  $report = $report_list[arg(3)];
  if ($report) {
    $callback = $report['callback'];
    if ($callback && function_exists($callback)) {
      return $callback();
    }
  }
  else if (! arg(3) || arg(3) == 'main') {
    return seo_friend_main_report();
  }
  return t('Report %report is not configured properly.', array('%report' => arg(3)));
} // function seo_friend_reports

/**
 * Generate main SEO report. Shows list of other available reports and status of
 * SEO-related modules.
 */
function seo_friend_main_report() {
  _seo_friend_add_admin_css();
  $report_list = module_invoke_all('seo_reports');
  asort($report_list);
  $output = '<strong>'.t('SEO Reports').'</strong>';
  $output .= '<ul>';
  foreach ($report_list as $report => $report_data) {
    if ($report) {
      $title = $report_data['title'] ? $report_data['title'] : $report.' Report'; 
      $output .= '<li>'.l($title,'admin/reports/seo/'.$report).'</li>';
    }
  }
  $output .= '</ul>';

  // notify if more reports should be added
  $output .= '<p><strong>'.t('Add More SEO Reports').'</strong><br/>';
  $output .= t('If you would like additional SEO reports added, please 
      log an issue at the drupal.org project page or email me at !email. If you 
      have custom SEO reports that you want to show up here, you can implement 
      the hook_seo_reports function.',
    array('!email' => '<a href="mailto:seo_friend@kristen.org">seo_friend@kristen.org</a>')).'</p>';

  // list of seo-related modules that have been or could be installed
  $output .= '<strong>'.t('SEO-Related Modules').'</strong>';
  $seo_modules = module_invoke_all('seo_modules');
  ksort($seo_modules);
  $rows = array();
  foreach ($seo_modules as $module => $module_data) {
    if (module_exists($module)) {
      $msg = t('This module is installed.');
      $class = 'ok';
    }
    else {
      $msg = seo_friend_module_not_found($module);
      if ($module_data['uninstalled_error']) {
        $msg .= ' <b>'.t('Note').'</b>: '.$module_data['uninstalled_error'];
        $class = 'error';
      }
      else {
        $class = 'warning';
      }
    }
    $project_page = seo_friend_get_project_page_link($module);
    $rows[] = array(
      'class' => $class,
      'data' => array(array('data' => $module, 'class' => 'row-status'), $module_data['description'], $msg, $project_page),
    );
  }
  $output .= theme('table', array(t('Module'), t('Description'), t('Status'), t('Project Page')), $rows, array('class' => 'system-status-report seo-friend-admin'));

  // notify if more modules should be added
  $output .= '<p><strong>'.t('Add More SEO-Related Modules').'</strong><br/>';
  $output .= t('If you know of other SEO-related modules that should be
      permanently added to this list, please log an issue at the drupal.org
      project page or email me at !email. If you have custom modules that you
      want to show up here, you can implement the hook_seo_modules function.',
    array('!email' => '<a href="mailto:seo_friend@kristen.org">seo_friend@kristen.org</a>')).'</p>';

  return $output;
} // function seo_friend_main_report

/**
 * Get a simple string version of the given array. If the array contains arrays,
 * just serialize the nested arrays.
 *
 * @param $data The array to make into a string.
 */
function seo_friend_get_array_as_string($data) {
  if (is_array($data)) {
    $tmp_data = array();
    foreach ($data as $data_key => $data_value) {
      if (is_array($data_value)) {
        $data_value = serialize($data_value);
      }
      $tmp_data[] = $data_key.'='.$data_value;
    }
    $data = implode(', ', $tmp_data);
  }
  return $data;
}

/**
 * Generate a nodewords report. Shows all existing meta tags for the nodes in
 * the system. Indicates if there is missing or duplicate meta tag content.
 *
 * fixme: nodewords module now allows tags for users and paths so handle these!
 */
function seo_friend_nodewords_report() {
  if (module_exists('nodewords')) {
    $header = array(
        array('data' => t('nid'), 'field' => 'n.nid'),
        array('data' => t('status'), 'field' => 'n.status'),
        array('data' => t('title'), 'field' => 'r.title'),
        array('data' => t('meta tags')),
        array('data' => t('edit')),
        );
    $nodes = seo_friend_get_all_nodes(TRUE, $header);
    $content = array();
    $meta_tags = array();
    $results = db_query('
        SELECT n.nid, n.type as node_type, w.name, w.content, w.type
        FROM {node} AS n
        INNER JOIN {nodewords} AS w ON n.nid=w.id WHERE (w.type="node" OR
        w.type=%d) AND 
        n.nid IN ('.db_placeholders(array_keys($nodes), 'int').')', 
          array_merge(array(NODEWORDS_TYPE_NODE), array_keys($nodes)));
    while ($row = db_fetch_object($results)) {
      $use_default = FALSE;
      if (is_string($row->content) && preg_match('/^a:\d+:/', $row->content, $matches)) {
        $row_content_array = unserialize($row->content);
        if (is_array($row_content_array) && isset($row_content_array['value'])) {
          $row->content = $row_content_array['value'];
          if ($row_content_array['use_default']) {
            $use_default = TRUE;
          }
        }
      }
      if ($use_default) {
        $row->content = 'DEFAULT';
        if ($row->name == 'robots') {
          $robots_default = variable_get('nodewords_list_robots', null);
          if ($robots_default) {
            $row->content .= ' ('.seo_friend_get_array_as_string($robots_default).')';
          }
        }
      }
      else if (is_array($row->content)) {
        $row->content = seo_friend_get_array_as_string($row->content);
      }
      $meta_tags[$row->nid][$row->name] = $row->content;
      $content[$row->name][$row->content][] = $row->nid;
      $node_types[$row->nid] = $row->node_type;
    }
    foreach ($nodes as $node) {
      $class = 'ok';
      $row = array(array('data' => $node->nid, 'class' => 'row-status'), seo_friend_get_node_status($node), l($node->title, 'node/'.$node->nid));
      $node_tags = $meta_tags[$node->nid];
      if (is_array($node_tags)) {
        $rows2 = array();
        $tags = seo_friend_get_tags('nodewords');
        if ($tags) {
          foreach ($tags as $tag_name) {
            $tag_content = $node_tags[$tag_name] ? $node_tags[$tag_name] : 'MISSING';
            if ($tag_name == 'robots' && $tag_content == 'MISSING') {
              // support older nodewords
              $nodewords_data = variable_get('nodewords', null);
              if ($nodewords_data['global']['robots']) {
                $tag_content = 'DEFAULT'.' ('.$nodewords_data['global']['robots'].')';
              }
            }
            $duplicates = '';
            if ($tag_content != 'MISSING') {
              $dup_array = $content[$tag_name][$tag_content];
              $dup_type = seo_friend_get_override_type('nodewords', $node_types[$node->nid]);
              $duplicates_setting = variable_get(
                  seo_friend_get_key(SEO_DUPLICATES, 'nodewords', $tag_name, $dup_type), 
                  seo_friend_get_duplicate_select_default($tag_name)
                  );
              $duplicates = seo_friend_get_duplicates($dup_array, $node->nid, $duplicates_setting);
              if ($duplicates) {
                $class = 'error';
                $has_duplicates = TRUE;
              }
            }
            $rows2[] = array($tag_name, check_plain($tag_content), $duplicates);
          }
        }
        $row[] = theme('table', array('tag', 'value', 'duplicates'), $rows2);
      }
      else {
        $class = 'warning';
        $row[] = t('ALL TAGS MISSING');
      }
      $row[] = l('Edit', 'node/'.$node->nid.'/edit', array('query' => drupal_get_destination()));
      $row = array('data' => $row, 'class' => $class);
      $rows[] = $row;
    }
    if ($has_duplicates) {
      $rows = seo_friend_sort_by_duplicates($rows);
      $dup_msg = seo_friend_duplicate_message();
      drupal_set_message($dup_msg, 'error');
    }
    $overview = '<p><strong>'. t('Here is the meta tag content you have set up with nodewords. Search for MISSING to see which nodes are missing meta tag content.') .'</strong></p>';

    _seo_friend_add_admin_css();
    return $overview . theme('table', $header, $rows, array('class' => 'seo-friend-admin')).theme('pager', array(), seo_friend_get_number_variable(SEO_PAGER_NUM, 100));
  }
  else {
    return seo_friend_module_not_found('nodewords');
  }
} // function seo_friend_nodewords_report

/**
 * Sort for duplicates or meta tags column.
 *
 * @param $data The table data to sort.
 */
function seo_friend_sort_by_duplicates($data) {
  // sort the data if duplicates header link has been clicked
  if (! isset($_GET['order'])) {
    $_GET['order'] = 'duplicates';
    $_GET['sort'] = 'asc';
  }
  if (in_array($_GET['order'], array('duplicates', 'meta tags'))) {
    $dups_array = array();
    $non_dups_array = array();
    foreach ($data as $sort_data) {
      if ($sort_data['class'] == 'error') {
        $dups_array[] = $sort_data;
      }
      else {
        $non_dups_array[] = $sort_data;
      }
    }
    if ($_GET['sort'] == 'asc') {
      $data = array_merge($dups_array, $non_dups_array);
    }
    else if ($_GET['sort'] == 'desc') {
      $data = array_merge($non_dups_array, $dups_array);
    }
  }
  return $data;
}

/**
 * Generates page_title report. Shows list of nodes with node title and page
 * title.
 */
function seo_friend_page_title_report() {
  if (module_exists('page_title')) {
    $id_column_name = seo_friend_get_page_title_id_column();
    if (! $id_column_name) {
      return;
    }
    $header = array(
        array('data' => 'nid', 'field' => 'n.nid'),
        array('data' => 'status', 'field' => 'n.status'),
        array('data' => 'node title', 'field' => 'r.title'),
        array('data' => 'page title', 'field' => 'p.page_title'),
        array('data' => 'duplicates'),
        array('data' => 'edit'),
        );
    $nodes = seo_friend_get_all_nodes(TRUE, $header);
    foreach ($nodes as $node) {
      $node_titles[$node->title][] = $node->nid;
      $page_titles[$node->page_title][] = $node->nid;
    }
    $has_duplicates = FALSE;
    $duplicate_types = array();
    foreach ($nodes as $node) {
      $page_title = $node->page_title;
      $node_title_dups = seo_friend_get_duplicates($node_titles[$node->title], $node->nid);
      if ($node_title_dups) {
        $duplicate_types['title'] = 'title';
        $has_duplicates = TRUE;
      }
      if ($page_title) {
        $page_title_dups = seo_friend_get_duplicates($page_titles[$page_title], $node->nid);
        if ($page_title_dups) {
          $has_duplicates = TRUE;
          $duplicate_types['page title'] = 'page title'; 
        }
        $duplicates = $node_title_dups ? 
          $node_title_dups . ($page_title_dups ? ','.$page_title_dups : '') :
          $page_title_dups;
        $data[] = array(
          'class' => $duplicates ? 'error' : 'ok',
          'data' => array(array('data' => $node->nid, 'class' => 'row-status'), seo_friend_get_node_status($node), l($node->title, 'node/'.$node->nid), check_plain($page_title), $duplicates, l('Edit', 'node/'.$node->nid.'/edit', array('query' => drupal_get_destination()))),
        );
      }
      else {
        // figure out what default page title pattern is used
        $pattern = variable_get('page_title_type_'. (isset($node->type) ? $node->type : ''), '');
        if (empty($pattern)) {
          $pattern = variable_get('page_title_default', '[page-title] | [site-name]');
        }
        $page_title_msg = t('Auto-created from pattern: @pattern', array('@pattern'=>$pattern));

        $data[] = array(
          'class' => $node_title_dups ? 'error' : 'ok',
          'data' => array(array('data' => $node->nid, 'class' => 'row-status'), seo_friend_get_node_status($node), l($node->title, 'node/'.$node->nid), $page_title_msg, $node_title_dups, l('Edit', 'node/'.$node->nid.'/edit', array('query' => drupal_get_destination()))),
        );
      }
    }
    if ($has_duplicates) {
      $data = seo_friend_sort_by_duplicates($data);
      $dup_msg = seo_friend_duplicate_message('nodes', implode(' and ', $duplicate_types));
      drupal_set_message($dup_msg, 'error');
    }
    $overview = '<p><strong>'. t('Here is the title content you have set up with page_title.') .'</strong></p>';
    _seo_friend_add_admin_css();
    return $overview . theme('table', $header, $data, array('class' => 'seo-friend-admin')).theme('pager', array(), seo_friend_get_number_variable(SEO_PAGER_NUM, 100));
  }
  else {
    return seo_friend_module_not_found('page_title');
  }
} // function seo_friend_page_title_report

/**
 * Gets the duplicates associated with the given duplicate array for the given
 * node id.
 *
 * @param $dup_array The nid array to check.
 * @param $nid The node id.
 */
function seo_friend_get_duplicates($dup_array, $nid, $duplicates_setting = 'warning') {
  if ($duplicates_setting != 'nocheckreport') {
    $duplicates = '';
    if (! empty($dup_array) && sizeof($dup_array) > 1) {
      $tmp = array();
      foreach ($dup_array as $d) {
        // don't include given nid
        if ($d != $nid) {
          $tmp[] = $d;
        }
      }
      $duplicates = implode(',', $tmp);
    }
  }
  return $duplicates;
}

/**
 * Generate a nodewords_bypath report. Shows all existing meta tag rules and a
 * list of system navigation paths that could be updated with meta tags.
 * Indicates if there are duplicates.
 */
function seo_friend_nodewords_bypath_report() {
  if (module_exists('nodewords_bypath')) {
    // get all url aliases that are not for nodes
    $results = db_query('
        SELECT r.id, r.path_expr, r.name, t.meta_tag, t.meta_value
        FROM {nodewords_bypath_rules} AS r
        INNER JOIN {nodewords_bypath_tags} AS t ON r.id=t.rule_id');
    while ($row = db_fetch_object($results)) {
      $content[$row->id][$row->meta_tag] = $row->meta_value;
      $name[$row->id] = $row->name;
      $paths[$row->id] = $row->path_expr;
      $meta_content[$row->meta_tag][$row->meta_value][] = $row->id;
    }
    if (empty($content)) {
      return t('No nodewords_bypath content');
    }
    foreach ($content as $id => $data) {

      // check if there are multiple paths
      $tmp = explode('|', preg_replace('/(\r\n?|\n)/', '|', $paths[$id]));
      if (sizeof($tmp) > 1) {
        $multiple = TRUE;
        foreach ($tmp as $path) {
          $rows[] = array($path);
        }
        $show_path = theme('table', array(''), $rows);
      }
      else {
        $show_path = $paths[$id];
      }

      $meta = array();
      $class = 'ok';
      foreach ($data as $meta_tag => $meta_value) {
        $duplicates = seo_friend_get_duplicates($meta_content[$meta_tag][$meta_value], $id);
        $class = ($duplicates || $multiple || $class == 'error') ? 'error' : 'ok';
        $meta[] = array($meta_tag, check_plain($meta_value), $duplicates);
      }
      $meta_tags = theme('table', array(t('id'), t('meta tags'), t('duplicates')), $meta);
      $edit = l('Edit', 'admin/content/nodewords/path/'.$id.'/edit', array('query' => drupal_get_destination()));

      $show[] = array(
          'data' => array(array('data' => $id, 'class' => 'row-status'), check_plain($name[$id]), $show_path, $meta_tags, $edit),
          'class' => $class
          );
    }
    $header = array(t('rule id'), t('rule name'), t('paths'), t('meta tags'), t('edit'));
    if ($multiple) {
      $mult_msg = seo_friend_duplicate_message('paths');
      drupal_set_message($mult_msg, 'error');
    }
    $overview = '<p><strong>'. t('Here is the meta tag content you have set up with nodewords_bypath.') .'</strong></p>';
    $output = $overview . theme('table', $header, $show, array('class' => 'seo-friend-admin'));

    // list of links that could add meta data for
    $results = db_query("
        SELECT link_path 
        FROM {menu_links} 
        WHERE module='system' AND 
        menu_name='navigation' AND 
        link_path NOT LIKE 'node%' AND 
        link_path NOT LIKE 'admin%' AND 
        link_path NOT LIKE 'user%' AND 
        link_path NOT LIKE 'filter%' AND 
        link_path NOT LIKE 'system%' AND 
        link_path NOT LIKE 'logout%' AND 
        link_path NOT LIKE 'batch%' AND 
        link_path NOT LIKE 'rss.xml%' AND 
        link_path NOT LIKE '%/%%%'");
        
    while ($row = db_fetch_object($results)) {
      $links[] = $row->link_path;
      foreach ($paths as $id => $pages) {
        if (drupal_match_path($row->link_path, $pages)) {
          $matches[$row->link_path] = $id;
        }
      }
    }
    if ($links) {
      $header = array('link', 'rule id');
      $rows = array();
      foreach ($links as $link) {
        $rows[] = array($link, $matches[$link]);
      }
      $overview = '<p><strong>'.t('Here are some of the system navigation links added by views and
          other modules. Look through these to see if you want to add meta tags to
          any of these pages. You can add new meta tags here: ').
        l('nodewords_bypath add page', 'admin/content/nodewords/path/new').
        '</strong></p>';
      $output .= $overview . theme('table', $header, $rows);
    }

    _seo_friend_add_admin_css();
    return $output;
  }
  else {
    return seo_friend_module_not_found('nodewords_bypath');
  }
} // function seo_friend_nodewords_bypath_report

define ('PATHAUTO_UPDATE_DO_NOTHING', 'Do nothing. Leave the old alias intact.');
define ('PATHAUTO_UPDATE_CREATE_LEAVE', 'Create a new alias. Leave the existing alias functioning.');
define ('PATHAUTO_UPDATE_CREATE_DELETE', 'Create a new alias. Delete the old alias.');
define ('PATHAUTO_UPDATE_CREATE_REDIRECT', 'Create a new alias. Redirect from old alias.');

/**
 * Generates pathauto report. Checks that settings are optimal.
 */
function seo_friend_pathauto_report() {
  if (module_exists('pathauto')) {
    $update_action = variable_get('pathauto_update_action', -1);
    if ($update_action >= 0) {
      $optimal = FALSE;
      switch ($update_action) {
        case 0: 
          {
            $update_action_label = t(PATHAUTO_UPDATE_DO_NOTHING); 
            // fall through
          }
        case 3: 
          {
            if (! $update_action_label) {
              $update_action_label = t(PATHAUTO_UPDATE_CREATE_REDIRECT);
            }
            $optimal = TRUE;
            break;
          }
        case 1: 
          {
            $update_action_label = t(PATHAUTO_UPDATE_CREATE_LEAVE);
            $warning = t('With your current pathauto setting, you may end up with more than one path pointing to the exact same content. If only one of the paths is used throughout the site, this may not an issue internally, but sometimes old paths and new paths get intermingled and the search engines end up seeing both paths which dilutes your ranking. If old paths were available when the content was indexed by the search engines, those will stay indexed for some time until the search engine figures out the content has been orphaned (assuming there are no internal links to the content).');
            break;
          }
        case 2: 
          {
            $update_action_label = t(PATHAUTO_UPDATE_CREATE_DELETE);
            $warning = t('With your current pathauto setting, you may end up with broken links on your site. If you are diligent about regular monitoring for broken links or only ever use /node/[nid] urls, this may not be an issue internally, but there may be search engines and other sites referencing old paths. When users go to these old paths, they will get a 404 error. If you have the Search 404 module installed, the user may end up finding the desired content but this roundabout process is error prone.');
            break;
          }
        default: 
          {
            $error = t('Your pathauto update action is misconfigured.');
          }
      }
      $output .= '<h3>'.t('Update Action').'</h3><br/>';
      if ($error) {
        $output .= $error;
      }
      else {
        $output .= '<b>'.t('Current setting:').'</b> <i>'.$update_action_label.'</i><br/><br/>';
        if ($optimal) {
          $output .= '<b>'.t('Diagnosis:').'</b> '.t('Your pathauto update action is set to an optimal setting.').'<br/><br/>';
          if ($update_action == 0) {
            $output .= '<b>'.t('Recommendation:').'</b> '.t('You are fine leaving your setting as is though you might consider using the redirect setting "%create_redirect" if you want your paths to accurately reflect your current content and/or you plan on changing your pathauto patterns in the future. During development, it makes sense to use the redirect setting while you are fine-tuning your pathauto patterns so you make sure that you are seeing your latest changes (alternatively, you can use the "%create_delete" but this setting is not recommended when the site is live). The only reason you might not want to use the "%create_redirect" setting is that it will end up creating as many redirects as applicable, so if you have a very large site where content is changed often (such that it affects the paths), you may end up with a very large number of redirects in your redirect table. For most sites, this is not an issue though.', array('%create_redirect' => t(PATHAUTO_UPDATE_CREATE_REDIRECT), '%create_delete' => t(PATHAUTO_UPDATE_CREATE_DELETE)));
            if (!function_exists('path_redirect_save')) {
              $output .= ' <b>'.t('Note').':</b> '.t('In order to use the "%create_redirect" setting, you must have the path_redirect module installed and enabled.', array('%create_redirect' => t(PATHAUTO_UPDATE_CREATE_REDIRECT))).' '.seo_friend_module_not_found('path_redirect');
            }
          }
        }
        else {
          $output .= '<b>'.t('Diagnosis:').'</b> '.$warning.'<br/><br/>';
          $output .= '<b>'.t('Recommendation:').'</b> '.t('It is recommended that you change your pathauto update action settings to be one of the following').':<br/>';
          $output .= '<ul>';
          $output .= '<li><i>'.t(PATHAUTO_UPDATE_DO_NOTHING).'</i></li>';
          $output .=
            '<li><i>'.t(PATHAUTO_UPDATE_CREATE_REDIRECT).'</i> <b>('.t('Recommended').')</b> ';
          if (! function_exists('path_redirect_save')) {
            $output .= ' ('.t('Requires path_redirect module.').' '.seo_friend_module_not_found('path_redirect').')';
          }
          $output .= '</li>';
          $output .= '</ul><br/>';
        }
      }
    }
    return $output;
  }
  else {
    return seo_friend_module_not_found('pathauto');
  }
}

/**
 * Gets a message to show when there are duplicates.
 *
 * @param $pages The page type, e.g. 'nodes' or 'paths'.
 * @param $type The type of duplicate content, e.g. 'meta tag' or 'page title'.
 */
function seo_friend_duplicate_message($pages = 'nodes', $type = 'meta tag') {
  return t('You currently are using the same @type content for multiple @pages. For optimal SEO, it is recommended that each page should have its own @type content.',
      array('@type'=>$type, '@pages'=>$pages));
}

/**
 * Gets all nids (with status & title) from the database.
 */
function seo_friend_get_all_nodes($use_pager = FALSE, $header = array()) {
  // all nodes
  $nodes = array();
  $page_title_field = '';
  $page_title_join = '';
  if (module_exists('page_title')) {
    $page_title_field = ', p.page_title ';
    $page_title_join = ' LEFT JOIN {page_title} AS p ON n.nid=p.'.seo_friend_get_page_title_id_column();
  }
  $sql = '
      SELECT n.nid, n.status, n.type, r.title, n.sticky '.$page_title_field.' 
      FROM {node} AS n
      INNER JOIN {node_revisions} AS r ON n.vid=r.vid '.$page_title_join.
      tablesort_sql($header);
  if ($use_pager) {
    $count_sql = '
      SELECT COUNT(n.nid) AS row_count
      FROM {node} AS n
      INNER JOIN {node_revisions} AS r ON n.vid=r.vid '.$page_title_join.
      tablesort_sql($header);
    $pager_num = seo_friend_get_number_variable(SEO_PAGER_NUM, 100);
    $results = pager_query($sql, $pager_num, 0, $count_sql);
  }
  else {
    $results = db_query($sql);
  }
  while ($node = db_fetch_object($results)) {
    $nodes[$node->nid] = $node;
  }
  return $nodes;
} // function seo_friend_get_all_nodes

/**
 * Add admin css.
 */
function _seo_friend_add_admin_css() {
  drupal_add_css(drupal_get_path('module', 'seo_friend') .'/seo_friend.admin.css', 'module', 'all', FALSE);
}
