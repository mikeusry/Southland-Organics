<?php

/**
 * @file
 * Advanced catalog browse pages.
 */

/**
 * Display a formatted catalog page.
 *
 * This function is a copy of original theme_uc_catalog_browse() with filter, pager
 * and products display options.
 *
 * If the category has products in it, display them. Subcategories
 *   are linked along the top of the page. If it does not have products, display
 *   subcategories in a grid with their images and subcategories.
 *
 * @param $tid
 *   Catalog term id from URL.
 * @return
 *   Formatted HTML of the catalog page using view.
 */
function uc_advanced_catalog_browse($tid = 0) {

  // original code from theme_uc_catalog_browse() on uc_catalog.pages.inc
  drupal_add_css(drupal_get_path('module', 'uc_catalog') .'/uc_catalog.css');
  $output = '';
  $catalog = uc_catalog_get_page((int)$tid);
  drupal_set_title(check_plain($catalog->name));
  drupal_set_breadcrumb(uc_catalog_set_breadcrumb($catalog->tid));
  $types = uc_product_types();
  $links = array();
  $child_list = array();
  foreach ($catalog->children as $child) {
    if (!empty($child->nodes)) {
      $links[] = array('title' => $child->name . (variable_get('uc_catalog_breadcrumb_nodecount', FALSE) ? ' ('. $child->nodes .')' : ''), 'href' => uc_catalog_path($child),
        'attributes' => array('rel' => 'tag'),
      );
    }
    if (!empty($child->image)) {
      $image = '<div>';
      if (module_exists('imagecache')) {
        $image .= l(theme('imagecache', 'uc_category', $child->image['filepath']), uc_catalog_path($child), array('html' => TRUE));
      }
      else {
        $image .= l(theme('image', $child->image['filepath']), uc_catalog_path($child), array('html' => TRUE));
      }
      $image .= '</div>';
    }
    else {
      $image = '<div></div>';
    }
    $grandchildren = array();
    $j = 0;
    $max_gc_display = 3;
    foreach ($child->children as $i => $grandchild) {
      if ($j > $max_gc_display) {
        break;
      }
      $g_child_nodes = 0;
      foreach ($types as $type) {
        $g_child_nodes += taxonomy_term_count_nodes($grandchild->tid, $type);
      }
      if ($g_child_nodes) {
        $grandchildren[$i] = l($grandchild->name, uc_catalog_path($grandchild), array('class' => 'subcategory'));
        $j++;
      }
    }
    //$grandchildren = array_slice($grandchildren, 0, intval(count($grandchildren) / 2) + 1, TRUE);
    if ($j > $max_gc_display) {
      array_push($grandchildren, l(t('More...'), uc_catalog_path($child), array('class' => 'subcategory')));
    }
    if ($child->nodes) {
      $cell_link = $image .'<strong>'. l($child->name, uc_catalog_path($child)) .'</strong>';
      if (variable_get('uc_catalog_show_subcategories', TRUE)) {
        $cell_link .= "<br/><span>". implode(', ', $grandchildren) ."</span>\n";
      }
      $child_list[] = $cell_link;
    }
  }
  if (!empty($catalog->image)) {
    if (module_exists('imagecache')) {
      $output .= theme('imagecache', 'uc_thumbnail', $catalog->image['filepath'], $catalog->name, $catalog->name, array('class' => 'category'));
    }
    else {
      $output .= theme('image', $catalog->image['filepath'], $catalog->name, $catalog->name, array('class' => 'category'));
    }
  }
  // ADDED: advanced catalog code
  drupal_add_css(drupal_get_path('module', 'uc_advanced_catalog') .'/uc_advanced_catalog.css');

  // session and request handler for default values
  $defaults = _uc_advanced_catalog_default();

  // Get products list
  $products = uc_advanced_catalog_products($catalog, $defaults, $types, $links);
  if ($products) {
    $output .= $products;
  }
  else {
    // Original code from theme_uc_catalog_browse()
    // Display table of child categories similar to an osCommerce site's front page.
    $columns = variable_get('uc_catalog_category_columns', 3);
    $cat_rows = array();
    $row = array();
    $i = 1;
    foreach ($child_list as $cell) {
      $row[] = array('data' => $cell, 'class' => 'category');
      if ($i % $columns == 0) {
        $cat_rows[] = $row;
        $row = array();
      }
      $i++;
    }
    if (count($row) > 0 && count($row) < $columns) {
      if (count($cat_rows) >= 1) {
        $row = array_merge($row, array_fill(count($row), $columns - count($row), array('data' => '&nbsp;', 'class' => 'category')));
      }
      $cat_rows[] = $row;
    }
    $output .= $catalog->description;
    $output .= theme('table', array(), $cat_rows, array('class' => 'category'));
    // end of original code
  }
  return $output;
}

/**
 * Construct products display with pager and filters
 *
 * @param $catalog
 *   object() catalog informations.
 * @param $defaults
 *   array() filters settings by request or session.
 * @param $types
 *   array() products node types.
 * @param $links
 *   array() sub_category links.
 * @return
 *   html catalog output.
 */
function uc_advanced_catalog_products($catalog, $defaults = array(), $types = array(), $links = array()) {
  $output = '';
  $products = array();

  // sort
  $sorts = array('ASC', 'DESC');
  if ($defaults['sort'] && in_array($defaults['sort'], $sorts)) {
    $sort = $defaults['sort'];
  }
  else {
    $sort = 'DESC';
  }
  // order
  $orders = array(
    'ordering' => 'uc_products.ordering '. $sort,
    'title' => 'n.title '. $sort,
    'sell_price' => 'uc_products.sell_price '. $sort,
    'weight' => 'uc_products.weight '. $sort,
    'length' => 'uc_products.length '. $sort,
    'width' => 'uc_products.width '. $sort,
    'height' => 'uc_products.height '. $sort,
  );
  if ($defaults['order'] && isset($orders[$defaults['order']])) {
    $order = $orders[$defaults['order']];
  }
  else {
    $order = $orders['ordering'];
  }
  // adding second order field
  // @TODO: better order managment
  if ($order == $orders['title']) {
    $order .= ', uc_products.ordering DESC, n.nid DESC';
  }
  else {
    $order .= ', n.title ASC';
  }

  // limit
  if ($defaults['limit'] && $defaults['limit'] == t('all')) {
    $limit = variable_get('uc_advanced_catalog_limit_settings_all', 100);
  }
  elseif ($defaults['limit']) {
    $limit = $defaults['limit'];
  }
  else {
    // default limit
    $limit = 12;
  }

  // SQL request construction
  $and = '';
  $sql_args = array($catalog->tid);
  $select = "SELECT DISTINCT(n.nid)";
  $select_count = "SELECT COUNT(DISTINCT(n.nid))";
  $from = " FROM {node} AS n"."\n";
  $inner = "  INNER JOIN {term_node} AS term_node ON n.vid = term_node.vid \n";

  foreach ($defaults AS $key => $value) {
    // taxonomy handlers
    if (ereg('taxonomy_', $key) && $defaults[$key] && is_numeric($defaults[$key])) {
      $inner .= "  INNER JOIN {term_node} AS $key ON n.vid = $key.vid \n";
      $and .= " AND $key.tid = %d \n";
      $sql_args[] = $value;
    }
    // attributes handlers
    if (ereg('attributes_', $key) && $defaults[$key] && is_numeric($defaults[$key])) {
      $inner .= "  INNER JOIN {uc_product_options} AS $key ON n.nid = $key.nid \n";
      $and .= " AND $key.oid = %d \n";
      $sql_args[] = $value;
    }
    // cck handling
    if (ereg('cck_', $key) && $defaults[$key] && ($value <> 'all')) {
      //dsm($key.' => '.$value);
      $key = explode('-', str_replace('cck_', '', $key));
      $name = $key[0];
      $db = $key[1];
      $node_type = $key[2];
      // db storage: content_type_TYPE
      if ($db == 1) {
        $inner .= "  INNER JOIN {content_type_$node_type} AS $name ON n.vid = $name.vid \n";
        $and .= " AND ". $name .".". $name ."_value = '%s' \n";
        $sql_args[] = $value;
      }
      else {
      // table storage
        $inner .= "  INNER JOIN {content_$name} AS $name ON n.vid = $name.vid \n";
        $and .= " AND ". $name .".". $name ."_value = '%s' \n";
        $sql_args[] = $value;
      }
    }
  }

  $inner .= "  INNER JOIN {uc_products} AS uc_products ON n.vid = uc_products.vid \n";
  $where = " WHERE term_node.tid = %d \n";
  $and .= " AND n.status = 1 \n";
  $and .= " AND n.type IN (". db_placeholders($types, 'varchar') .") \n";
  $order = " ORDER BY " . $order;

  $sql = $select . $from . $inner . $where . $and . $order;
  $sql_count = $select_count . $from . $inner . $where . $and;
  $sql = db_rewrite_sql($sql);
  $sql_count = db_rewrite_sql($sql_count);

  foreach ($types as $type) {
    $sql_args[] = $type;
  }
  // dsm($sql);
  // dsm($sql_args);
  // pager query
  $result = pager_query($sql, $limit, 0, $sql_count, $sql_args);
  while ($node = db_fetch_object($result)) {
    $products[] = $node->nid;
  }

  // catalog sub categories
  if (count($links)) {
    $output .= theme('links', $links, array('class' => 'links inline uc-categories')) ."<br />\n";
  }
  // catalog description
  //@todo: add administrative options
  $output .= $catalog->description;

  // give some information to filters
  global $pager_total_items;
  $variables = array(
    'count' => count($products),
    'total' => $pager_total_items[0],
  );

  // creating filters
  $filters_form = drupal_get_form('uc_advanced_catalog_pager_form', $defaults, $variables);

  // pager and filters position
  $pager_position = variable_get('uc_advanced_catalog_pager', 'top-bottom');
  $filters_position = variable_get('uc_advanced_catalog_filters', 'top-bottom');
  // top of the products
  if ($filters_position == 'top' || $filters_position == 'top-bottom') {
    $output .= $filters_form;
  }
  if ($pager_position == 'top' || $pager_position == 'top-bottom') {
    $output .= theme('pager');
  }

  // products theming by mode
  if ($defaults['mode']) {
    $mode = $defaults['mode'];
  }
  else {
    // default mode
    $mode = 'grid';
  }
  if ($products) {
    $output .= theme('uc_advanced_catalog_products', $products, $mode);
  }
  else {
    // no products but sub categories => original catalog display
    if (count($links)) {
      return FALSE;
    } else {
      //no products, no sub category => message
      $output .= '<div class="no-products">'. t('No products are available in this category.') .'</div>';
    }
  }
  // bottom of the products
  if ($pager_position == 'bottom' || $pager_position == 'top-bottom') {
    $output .= theme('pager');
  }
  if ($filters_position == 'bottom' || $filters_position == 'top-bottom') {
    $output .= $filters_form;
  }

  return $output;
}

/**
 * Display a formatted list of products by mode.
 *
 * @param $products
 *   An array of product nids.
 * @param $mode
 *   String of display mode
 * @ingroup themeable
 */
function theme_uc_advanced_catalog_products($products, $mode) {
  $output = '<div class="category-'. $mode .'-products">';
  if (function_exists('theme_uc_advanced_catalog_products_'. $mode)) {
    $output .= theme('uc_advanced_catalog_products_'. $mode, $products);
  }
  else {
    $output .= t('error, no theming function for !mode mode!', array('!mode' => $mode));
  }
  $output .= "</div>";
  return $output;
}

/**
 * Display a formatted table of products.
 *
 * @param $products
 *   An array of product nids.
 * @ingroup themeable
 */
function theme_uc_advanced_catalog_products_table($products) {
  // ubercart original table display
  // some header alteration on uc_advanced_catalog.module
  $table = tapir_get_table('uc_product_table', $products);
  return drupal_render($table);
}

/**
 * Display a formatted list of products.
 *
 * @param $products
 *   An array of product nids.
 * @ingroup themeable
 */
function theme_uc_advanced_catalog_products_list($products) {
  $output = uc_advanced_catalog_products_rows($products, 'list');
  return $output;
}

/**
 * Display a formatted grid of products.
 *
 * @param $products
 *   An array of product nids.
 * @ingroup themeable
 */
function theme_uc_advanced_catalog_products_grid($products) {
  $output = uc_advanced_catalog_products_rows($products, 'grid');
  return $output;
}

/**
 * Construct formatted products for theme
 *
 * @param $products
 *   An array of product nids.
 * @param $build_mode
 *   String corresponding to build mode, 'grid' or 'list'
 * @return
 *   html products output.
 */
function uc_advanced_catalog_products_rows($products, $build_mode = 'grid') {
  $output = '';

  $rows = $row = array();
  if ($build_mode == 'grid') {
    $columns  = variable_get('uc_catalog_grid_display_width', 3);
    $row_count = 0;
  }

  $count = 0;
  foreach ($products as $nid) {
    // Use object_cache if possible to retrieve objects.
    if (module_exists('object_cache')) {
      $product = object_cache_get('node', $nid);
    }
    else {
      $product = node_load($nid);
    }

    //@todo : is this useful if no other cck than image ?
    if (module_exists('content')) {
      $product->build_mode = $build_mode;
      $product = node_build_content($product);
      drupal_render($product->content);
      content_alter($product);
    }

    if (module_exists('ds') && module_exists('nd')) {
      $show_links = ds_show_field('nd', $product->type, $build_mode, 'links');
      // use teaser display to get trim body
      $product = node_view($product, TRUE, FALSE, $show_links);
      $row[] = $product;
      if ($build_mode == 'grid') {
        $row_count ++;
        if (($count + 1) % $columns == 0) {
          $rows[] = $row;
          $row = array();
          $row_count = 0;
        }
      }
    }
    else {
      // manual theming ?
    }
    $count++;
  }
  if ($build_mode == 'grid') {
    if ($row) {
      // Fill up the last line, default true.
      if (variable_get('uc_advanced_catalog_grid_fill', TRUE)) {
        for ($i = 0; $i < ($columns - $row_count); $i++) {
          $row[] = '';
        }
      }
      $rows[] = $row;
    }
    $output .= theme('table', array(), $rows);
  }
  else {
    // list mode
    $output .= implode("\n", $row);
  }
  return $output;
}

/**
 * Implementation of hook_tapir_table_header_alter().
 */
function uc_advanced_catalog_tapir_table_header_alter(&$header, $table_id) {
  if ($table_id == 'uc_product_table') {
    if (variable_get('uc_advanced_catalog_table_header', t('Hide')) == t('Hide')) {
      // no more header
      foreach ($header AS &$value) {
        unset($value['cell']);
      }
    }
    else {
      // no more sort (handle by advanced catalog filters)
      foreach ($header AS &$value) {
        unset($value['cell']['field']);
      }
    }
  }
}

// function uc_advanced_catalog_tapir_table_alter(&$table, $table_id) {
  // if ($table_id == 'uc_product_table') {
  // }
// }
