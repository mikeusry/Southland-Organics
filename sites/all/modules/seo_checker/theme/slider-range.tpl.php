<?php
// $Id: slider-range.tpl.php,v 1.1.2.1 2009/03/25 13:00:57 miruoss Exp $

/**
 * @file
 * Default theme implementation to display the slider-range form element
 *
 * Available variables:
 * - $element: The element array containing #id, #name,...
 */

?>
<div>
  <strong><?php echo $element["#title"];?>:</strong>
</div>
<div style="float:left">
  <div id='<?php echo $element["#id"]; ?>_slider' class='ui-slider-1'></div>
</div>
<span style="margin-left: 5px;">[<span id='<?php echo $element["#id"]; ?>_lower_nr'><?php echo $element["#default_value"][0]; ?>%</span>,<span id='<?php echo $element["#id"]; ?>_upper_nr'><?php echo $element["#default_value"][1]; ?>%</span>]</span>
<div style="clear:both;"></div>
<script type="text/javascript">
<!--
  $('#<?php echo $element["#id"];?>_slider').slider({
    slide: changeHandle,
    steps: <?php echo $element["#steps"]; ?>,
    handles: [
      {
        start: <?php echo $element["#default_value"][0]; ?>,
        id: '<?php echo $element["#id"];?>_handle_lower'
      },
      {
        start: <?php echo $element["#default_value"][1]; ?>,
        id: '<?php echo $element["#id"];?>_handle_upper'
      }
    ],
    range: true
});
//-->
</script>
<div class="description" style="margin-bottom:10px;"><?php echo $element["#description"];?></div>
<input type="hidden" name='<?php echo $element["#name"]; ?>[]' id='<?php echo $element["#id"];?>_lower_value' value='<?php echo $element["#default_value"][0]; ?>' />
<input type="hidden" name='<?php echo $element["#name"]; ?>[]' id='<?php echo $element["#id"];?>_upper_value' value='<?php echo $element["#default_value"][1]; ?>' />