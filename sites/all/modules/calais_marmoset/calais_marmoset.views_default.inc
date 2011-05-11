<?php

/**
 * Implementation of hook_views_default_views().
 */
function calais_marmoset_views_default_views() {
  $view = new view;
  $view->name = 'marmoset';
  $view->description = 'for use with the marmoset module.  provides a url that the terms are mapped to';
  $view->tag = 'calais';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'title' => array(
      'label' => 'Title',
      'link_to_node' => 1,
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Override',
      ),
    ),
    'tid' => array(
      'label' => 'All terms',
      'type' => 'separator',
      'separator' => ', ',
      'empty' => '',
      'link_to_taxonomy' => 1,
      'limit' => 0,
      'vids' => array(
        '1' => 0,
        '2' => 0,
        '3' => 0,
        '4' => 0,
        '5' => 0,
        '6' => 0,
        '7' => 0,
        '8' => 0,
        '9' => 0,
        '10' => 0,
        '11' => 0,
        '12' => 0,
        '13' => 0,
        '14' => 0,
        '15' => 0,
        '16' => 0,
        '17' => 0,
        '18' => 0,
        '19' => 0,
        '20' => 0,
        '21' => 0,
        '22' => 0,
        '23' => 0,
        '24' => 0,
        '25' => 0,
        '26' => 0,
        '27' => 0,
        '28' => 0,
        '29' => 0,
        '30' => 0,
        '31' => 0,
        '32' => 0,
        '33' => 0,
        '34' => 0,
        '35' => 0,
        '36' => 0,
        '37' => 0,
        '38' => 0,
        '39' => 0,
      ),
      'exclude' => 0,
      'id' => 'tid',
      'table' => 'term_node',
      'field' => 'tid',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'created' => array(
      'label' => 'Post date',
      'date_format' => 'custom',
      'custom_date_format' => 'm/d/Y',
      'exclude' => 1,
      'id' => 'created',
      'table' => 'node',
      'field' => 'created',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Override',
      ),
    ),
    'teaser' => array(
      'label' => 'Teaser',
      'exclude' => 0,
      'id' => 'teaser',
      'table' => 'node_revisions',
      'field' => 'teaser',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'created' => array(
      'order' => 'DESC',
      'granularity' => 'second',
      'id' => 'created',
      'table' => 'node',
      'field' => 'created',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'name' => array(
      'id' => 'name',
      'table' => 'term_data',
      'field' => 'name',
    ),
  ));
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => 1,
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('empty', 'There are no items that match the provided term.');
  $handler->override_option('empty_format', '1');
  $handler->override_option('use_pager', '1');
  $handler->override_option('style_plugin', 'list');
  $handler->override_option('style_options', array(
    'grouping' => 'created',
    'type' => 'ul',
  ));
  $handler->override_option('row_options', array(
    'inline' => array(),
    'separator' => '',
  ));
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->override_option('path', 'marmoset');
  $handler->override_option('menu', array(
    'type' => 'none',
    'title' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'weight' => 0,
  ));
  $views[$view->name] = $view;

  return $views;
}
