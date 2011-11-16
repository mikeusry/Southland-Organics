
Advanced catalog add custom filer on ubercart catalog
 - Products per page
 - Ordering by position, name, price, weight, lenght, width or height.
 - Sort ascending or descending
 - Mode filter to display your catalog using display suite (node display).
 - taxonomy filters
 - cck filters
 - product attributes filter

*******************************************************************************************
Warning:

This module is in active development, currrently there is some restrictions:
Attributes by product class not supported
Only options select or checkboxes cck support (render as select)

*******************************************************************************************

Requirements
------------

This module require at least version 6.x of Drupal and Ubercart 6.x-2.4
Other dependencies
  * Display suite 6.x.1.4+: http://drupal.org/project/ds
  * Node displays 6.x.2.4: http://drupal.org/project/nd
  * Recommended: Object cache 6.x-1.0: http://drupal.org/project/object_cache

Installation
------------
1. Copy the folder named 'uc_advanced_catalog' and its contents to the Ubercart contrib modules 
   directory of your Drupal installation
   (for example 'sites/all/modules/ubercart/contrib/').
   
2. Go to 'admin/build/modules' and enable Ubercart advanced catalog.

3. Go to 'admin/store/settings/catalog/edit/advanced' and edit option for the module.

4. Configure node display for catalog grid and catalog list mode
   admin/build/ds/layout/product/grid
   admin/build/ds/layout/product/list

*******************************************************************************************

This module has been developed by Mog

Post a message on the drupal.org site if you have any ideas on 
how we can improve the module.

Mog.
tech@arthura.fr