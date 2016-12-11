$(document).ready(function () {
    $('.pagination a:eq(0)').click(function (e) {
        e.preventDefault();
        var key = $('meta[name=key]').attr("content"), page = parseInt($('meta[name=page]').attr("content"));
        if (page == 1) {
            $(this).unbind('click').parent().addClass('disabled');
            layer.alert("没有了！")
            return;
        }
        location.href = "/blog/search?key=" + key + "&page=" + (page - 1);
    });
    $('.pagination a:eq(1)').click(function (e) {
        e.preventDefault();
        var key = $('meta[name=key]').attr("content"), page = parseInt($('meta[name=page]').attr("content"));
        if ($('.media').length == 0) {
            $(this).unbind('click').parent().addClass('disabled');
            layer.alert("没有了！")
            return;
        }
        location.href = "/blog/search?key=" + key + "&page=" + (page + 1);
    });
});
