<?php
// $Id: slider-nojquery.tpl.php,v 1.1.2.1 2009/03/25 13:00:57 miruoss Exp $

/**
 * @file
 * Default theme implementation to display a slider if jquery_ui is not installed
 *
 * Available variables:
 * - $element: The element array containing #id, #name,...
 */

?>
<div>
  <strong><?php echo $element["#title"];?>:</strong>
</div>
<div>
<div class="description" style="margin-bottom:10px;"><?php echo $element["#description"];?></div>
<?php if ($element['#slider_type'] == 'at_least'): ?>
&ge;&nbsp;<input type="text" size="<?php echo $element['#size'] ?>" name='<?php echo $element["#name"]; ?>' id='<?php echo $element["#id"];?>_value' value='<?php echo $element["#default_value"][0]; ?>' />&nbsp;%
<?php else: ?>
&#8712;&nbsp;[<input type="text" size="<?php echo $element['#size'] ?>" name='<?php echo $element["#name"]; ?>[]' id='<?php echo $element["#id"];?>_lower_value' value='<?php echo $element["#default_value"][0]; ?>' />&nbsp;%
, <input type="text" size="<?php echo $element['#size'] ?>" name='<?php echo $element["#name"]; ?>[]' id='<?php echo $element["#id"];?>_upper_value' value='<?php echo $element["#default_value"][1]; ?>' />&nbsp;%
]
<?php endif; ?>
</div>