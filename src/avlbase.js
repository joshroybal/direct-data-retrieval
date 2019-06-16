function AddStyle()
{
   var stylesheet = document.getElementById("styleinfo");
   stylesheet.innerHTML = "<link rel='stylesheet' media='all' type='text/css' href='/includes/gradienttable.css'/>";

   var table = document.getElementById("my_table");
   table.classList.add("gradienttable-left-justify");
}
window.onload = AddStyle;
