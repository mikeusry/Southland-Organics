<?php

/**
 * @file
 * Display suite default settings.
 */

function _uc_advanced_catalog_ds_default_settings() {
  $data = array(
    'nd' => array(
      'product' => array(
        'grid' => array(
          'fields' => array(
            'product_image' => array(
              'region' => 'middle',
              'weight' => '-98',
              'format' => 'uc_advanced_catalog_image_grid',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'sell_price' => array(
              'region' => 'middle',
              'weight' => '-97',
              'format' => 'uc_advanced_catalog_sell_price',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'list_price' => array(
              'region' => 'disabled',
              'weight' => '-84',
              'format' => 'uc_advanced_catalog_list_price',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'cost_price' => array(
              'region' => 'disabled',
              'weight' => '-85',
              'format' => 'uc_advanced_catalog_cost_price',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'model' => array(
              'region' => 'middle',
              'weight' => '-99',
              'format' => 'ds_eval_code',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'weight' => array(
              'region' => 'disabled',
              'weight' => '-83',
              'format' => 'ds_eval_code',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'lenght' => array(
              'region' => 'disabled',
              'weight' => '-81',
              'format' => 'ds_eval_code',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'add_to_cart' => array(
              'region' => 'middle',
              'weight' => '-96',
              'format' => 'ds_eval_code',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'buy_it_now' => array(
              'region' => 'disabled',
              'weight' => '-82',
              'format' => 'ds_eval_code',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'stock' => array(
              'region' => 'disabled',
              'weight' => '-86',
              'format' => 'ds_eval_code',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'title' => array(
              'region' => 'middle',
              'weight' => '-100',
              'format' => 'nd_title_p_link',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'author' => array(
              'region' => 'disabled',
              'weight' => '-92',
              'format' => 'ds_author_nolink',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'links' => array(
              'region' => 'disabled',
              'weight' => '-93',
              'format' => '',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'read_more' => array(
              'region' => 'disabled',
              'weight' => '-94',
              'format' => 'ds_eval_code',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'post_date' => array(
              'region' => 'disabled',
              'weight' => '-95',
              'format' => 'nd_post_date_small',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'body' => array(
              'region' => 'disabled',
              'weight' => '-91',
              'format' => 'nd_bodyfield',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'terms_1' => array(
              'region' => 'disabled',
              'weight' => '-90',
              'format' => 'nd_terms_per_vocabulary_space',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'terms_2' => array(
              'region' => 'disabled',
              'weight' => '-87',
              'format' => 'nd_terms_per_vocabulary_space',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'terms' => array(
              'region' => 'disabled',
              'weight' => '-88',
              'format' => '',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'files' => array(
              'region' => 'disabled',
              'weight' => '-89',
              'format' => '',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
          ),
          'status' => 2,
          'region_styles' => array(
            'header' => '',
            'left' => '',
            'middle' => '',
            'right' => '',
            'footer' => '',
          ),
        ),
        'list' => array(
          'fields' => array(
            'product_image' => array(
              'region' => 'left',
              'weight' => '-100',
              'format' => 'uc_advanced_catalog_image_list',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'sell_price' => array(
              'region' => 'right',
              'weight' => '-95',
              'format' => 'uc_advanced_catalog_sell_price',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'list_price' => array(
              'region' => 'disabled',
              'weight' => '-92',
              'format' => 'uc_advanced_catalog_list_price',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'cost_price' => array(
              'region' => 'disabled',
              'weight' => '-85',
              'format' => 'uc_advanced_catalog_cost_price',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'model' => array(
              'region' => 'middle',
              'weight' => '-98',
              'format' => 'ds_eval_code',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'weight' => array(
              'region' => 'disabled',
              'weight' => '-84',
              'format' => 'ds_eval_code',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'lenght' => array(
              'region' => 'disabled',
              'weight' => '-82',
              'format' => 'ds_eval_code',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'add_to_cart' => array(
              'region' => 'disabled',
              'weight' => '-83',
              'format' => 'ds_eval_code',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'buy_it_now' => array(
              'region' => 'right',
              'weight' => '-94',
              'format' => 'ds_eval_code',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'stock' => array(
              'region' => 'right',
              'weight' => '-96',
              'format' => 'ds_eval_code',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'title' => array(
              'region' => 'middle',
              'weight' => '-99',
              'format' => 'nd_title_h2_block_link',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'author' => array(
              'region' => 'disabled',
              'weight' => '-90',
              'format' => 'ds_author_nolink',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'links' => array(
              'region' => 'disabled',
              'weight' => '-92',
              'format' => '',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'read_more' => array(
              'region' => 'footer',
              'weight' => '-93',
              'format' => 'ds_eval_code',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'post_date' => array(
              'region' => 'disabled',
              'weight' => '-91',
              'format' => 'nd_post_date_small',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'body' => array(
              'region' => 'middle',
              'weight' => '-97',
              'format' => 'nd_bodyfield',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'terms_1' => array(
              'region' => 'disabled',
              'weight' => '-89',
              'format' => 'nd_terms_per_vocabulary_space',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'terms_2' => array(
              'region' => 'disabled',
              'weight' => '-86',
              'format' => 'nd_terms_per_vocabulary_space',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'terms' => array(
              'region' => 'disabled',
              'weight' => '-87',
              'format' => '',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
            'files' => array(
              'region' => 'disabled',
              'weight' => '-88',
              'format' => '',
              'css-class' => '',
              'labelformat' => 'hidden',
            ),
          ),
          'status' => 2,
          'region_styles' => array(
            'header' => '',
            'left' => '',
            'middle' => '',
            'right' => '',
            'footer' => '',
          ),
        ),
      ),
    ),
  );
  return $data;
}



function _uc_advanced_catalog_ds_fields() {
  // non product type will be exclude
  $product_types = array_combine(uc_product_types(), uc_product_types());
  $node_types = array_keys(node_get_types('names'));
  $node_types = array_combine($node_types, $node_types);
  $exclude = array_diff_key($node_types, $product_types);

  $data = array(
    'nd' => array(
      // product fields
      'product_image' => array(
        'title' => t('Product image'),
        'type' => DS_FIELD_TYPE_THEME,
        'status' => DS_FIELD_STATUS_STATIC,
        'properties' => array(
          'formatters' => array(
            'uc_advanced_catalog_image_grid' => t('Grid mode: First image'),
            //'uc_advanced_catalog_image_table' => t('Table mode: First image'),
            'uc_advanced_catalog_image_list' => t('List mode: First image'),
            'uc_advanced_catalog_image_all' => t('All images (Ubercart full node)'),
          ),
        ),
        'exclude' => $exclude,
      ),
      'sell_price' => array(
        'title' => t('Sell price'),
        'type' => DS_FIELD_TYPE_THEME,
        'status' => DS_FIELD_STATUS_STATIC,
        'properties' => array(
          'formatters' => array(
            'uc_advanced_catalog_sell_price' => t('Price formatted'),
            'uc_advanced_catalog_sell_price_themed' => t('Price themed'),
          ),
        ),
        'exclude' => $exclude,
      ),
      'list_price' => array(
        'title' => t('List price'),
        'type' => DS_FIELD_TYPE_THEME,
        'status' => DS_FIELD_STATUS_STATIC,
        'properties' => array(
          'formatters' => array(
            'uc_advanced_catalog_list_price' => t('Price formatted'),
            'uc_advanced_catalog_list_price_themed' => t('Price themed'),
          ),
        ),
        'exclude' => $exclude,
      ),
      'cost_price' => array(
        'title' => t('Cost'),
        'type' => DS_FIELD_TYPE_THEME,
        'status' => DS_FIELD_STATUS_STATIC,
        'properties' => array(
          'formatters' => array(
            'uc_advanced_catalog_cost_price' => t('Price formatted'),
          ),
        ),
        'exclude' => $exclude,
      ),
      'model' => array(
        'title' => t('SKU'),
        'type' => DS_FIELD_TYPE_CODE,
        'status' => DS_FIELD_STATUS_STATIC,
        'properties' => array(
          'formatters' => array(
            'ds_eval_code' => t('Default'),
          ),
          'code' => '[model]',
          'token' => TRUE,
        ),
        'exclude' => $exclude,
      ),
      'weight' => array(
        'title' => t('Weight'),
        'type' => DS_FIELD_TYPE_CODE,
        'status' => DS_FIELD_STATUS_DEFAULT,
        'properties' => array(
          'formatters' => array(
            'ds_eval_code' => t('Default'),
          ),
          'code' => '[weight]',
          'token' => TRUE,
        ),
        'exclude' => $exclude,
      ),
      'lenght' => array(
        'title' => t('Lenght'),
        'type' => DS_FIELD_TYPE_CODE,
        'status' => DS_FIELD_STATUS_DEFAULT,
        'properties' => array(
          'formatters' => array(
            'ds_eval_code' => t('Default'),
          ),
          'code' => '[length-raw]x[width-raw]x[height-raw] [length_units]',
          'token' => TRUE,
        ),
        'exclude' => $exclude,
      ),
      'add_to_cart' => array(
        'title' => 'Add to cart',
        'type' => DS_FIELD_TYPE_CODE,
        'status' => DS_FIELD_STATUS_STATIC,
        'properties' => array(
          'formatters' => array(
            'ds_eval_code' => t('Default'),
          ),
          'code' => '<?php echo theme(\'uc_product_add_to_cart\', $object); ?>'),
        'exclude' => $exclude,
      ),
      'buy_it_now' => array(
        'title' => 'Buy it now',
        'type' => DS_FIELD_TYPE_CODE,
        'status' => DS_FIELD_STATUS_STATIC,
        'properties' => array(
          'formatters' => array(
            'ds_eval_code' => t('Default'),
          ),
          'code' => '<?php echo drupal_get_form(\'uc_catalog_buy_it_now_form_\'. $object->nid, $object); ?>'),
        'exclude' => $exclude,
      ),
      'stock' => array(
        'title' => 'Stock',
        'type' => DS_FIELD_TYPE_CODE,
        'status' => DS_FIELD_STATUS_DEFAULT,
        'properties' => array(
          'formatters' => array(
            'ds_eval_code' => t('Default'),
          ),
          'code' => '<?php
$stock = uc_stock_level($object->model);
if (module_exists(\'uc_stock\') && $stock) {
$stock_text = t(\'Only !stock left !\', array(\'!stock\' => $stock));
echo $stock_text;
}
?>'
        ),
        'exclude' => $exclude,
      ),
    ),
  );
  return $data;
}
