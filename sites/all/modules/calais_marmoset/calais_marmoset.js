Drupal.behaviors.calais_marmoset_show_terms = function (context) {
  $("body span.vcard, body > a").wrapAll("<div class='marmoset'></div>");
  $("div.marmoset a").wrap("<div class='tag'></div>");
  $("div.marmoset ").prepend("<a class='hide' href='#'>Hide</a>").click(function(){$(this).hide();});

  $('body div.marmoset').css( {
    "position":"absolute",
    "width":"180px",
    "border":"3px solid black",
    "margin":"10px",
    "padding":"5px",
    "z-index": "5",
    "background":"#ccc" }
  );
  $('body div.marmoset a').css( {
    "width":"90px",
    "padding":"5px",
    "clear":"both" }
  );
  $('body div.marmoset span.vcard').css({
    "width":"280px",
    "font-weight":"bold",
    "clear":"both"}
  );
  $('body div.marmoset .hide').css( {
    "float":"right",
    "padding":"0 5px 0 0",
    "width":"20px",
    "clear":"none" }
  );
};
