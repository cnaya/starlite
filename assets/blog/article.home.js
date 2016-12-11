$(document).ready(function () {
    var tpl = '{{#post_list}}\
                    <div class="media">\
                        <a class="media-left" href="/user/detail/{{post_author_id}}">\
                            <img src="{{user_image}}" alt="">\n\
                        </a>\n\
                        <div class="media-body">\n\
                            <h3 class="media-heading article_title"><a href="/blog/detail/{{post_id}}">{{post_title}}</a></h3>\
    <p>{{{post_abstract}}}</p>\
    <p>\n\
        <a href="/user/detail/{{post_author_id}}">{{post_author_name}}</a>\
        &nbsp;&nbsp;发布于\
        &nbsp;&nbsp;{{post_created}}\
        &nbsp;&nbsp;<span>阅读 {{post_visitor}}</span>\
    </p>\
</div>\
</div>\
{{/post_list}}\
';
    var more = $('meta[name=more]'), page_ele = $('meta[name=page]');
    $('#more').click(function (e) {
        e.preventDefault();
        if (parseInt(more.attr('content')) == 0) {
            layer.alert('没有啦！');
            return;
        }
        var page = parseInt(page_ele.attr('content')) + 1;
        var dialog = layer.load();
        $.post("/blog/home", {page: page}, function (result) {
            layer.close(dialog);
            if (result.more) {
                more.attr("content", "1");
            } else {
                more.attr("content", "0");
            }
            page_ele.attr("content", page);

            $('.media:last').append(Mustache.render(tpl, result));
        });
    });
});


