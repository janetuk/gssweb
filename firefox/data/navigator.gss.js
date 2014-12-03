console.log("Bob is here. #2");

function addScript(url) {
  var elt = document.createElement("script");
  elt.setAttribute("src",   url );
  elt.setAttribute('type', 'text/javascript');
  document.body.appendChild(elt);
}

addScript('chrome://gssweb/content/nav.gss.js');


