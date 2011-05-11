function changeHandle(e,ui) {
  var id = ui.handle.attr('id').split('_handle').join('');
  $("#"+id+"_value").val(ui.value);
  $("#"+id+"_nr").text(ui.value+"%");
}