Drupal.keyword_module_changed = function (event) {
  module = $("input[name='keyword_rules_keywords_driver']:radio:checked").val();
  $('.keyword_rule_settings_fieldset').slideUp();
  $('#'+module+'_settings').slideDown();
};

$('document').ready(function () {
  module = $("input[name='keyword_rules_keywords_driver']:radio:checked").val();
  $('#'+module+'_settings').show();
  $("input[name='keyword_rules_keywords_driver']:radio").change(Drupal.keyword_module_changed);
});
