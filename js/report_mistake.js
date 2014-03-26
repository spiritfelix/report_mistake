jQuery(function($) {
  // script here
  $('.mistake').hide();
  $('.field  .field-item').hover(
    function() {
      $(this).find('.mistake').show();
    }, function() {
      $(this).find('.mistake').hide();
    }
  );
  $('.mistake').hover(function(){
    $(this).toggleClass('mistake-hover');
  });

  /**
  * Provide the HTML to create the modal dialog.
  */
  Drupal.theme.prototype.ReportMistakeModal = function () {
    var html = '';
    html += '<div id="ctools-modal" class="popups-box">';
    html += '  <div class="ctools-modal-content ctools-report-mistake-modal-content">';
    html += '    <table cellpadding="0" cellspacing="0" id="ctools-face-table">';
    html += '      <tr>';
    html += '        <td class="popups-tl popups-border"></td>';
    html += '        <td class="popups-t popups-border"></td>';
    html += '        <td class="popups-tr popups-border"></td>';
    html += '      </tr>';
    html += '      <tr>';
    html += '        <td class="popups-cl popups-border"></td>';
    html += '        <td class="popups-c" valign="top">';
    html += '          <div class="popups-container">';
    html += '            <div class="modal-header popups-title">';
    html += '              <span id="modal-title" class="modal-title"></span>';
    html += '              <span class="popups-close"><a class="close" href="#">' + Drupal.CTools.Modal.currentSettings.closeText + '</a></span>';
    html += '              <div class="clear-block"></div>';
    html += '            </div>';
    html += '            <div class="modal-scroll"><div id="modal-content" class="modal-content popups-body"></div></div>';
    html += '            <div class="popups-buttons"></div>';
    html += '            <div class="popups-footer"></div>';
    html += '          </div>';
    html += '        </td>';
    html += '        <td class="popups-cr popups-border"></td>';
    html += '      </tr>';
    html += '      <tr>';
    html += '        <td class="popups-bl popups-border"></td>';
    html += '        <td class="popups-b popups-border"></td>';
    html += '        <td class="popups-br popups-border"></td>';
    html += '      </tr>';
    html += '    </table>';
    html += '  </div>';
    html += '</div>';
    return html;
  }
  // end of script
});
