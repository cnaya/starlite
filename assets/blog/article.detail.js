$(document).ready(function () {
    var dialog = layer.load();
    $.post(location.href, function (result) {
        layer.close(dialog);
        if (!result.ok) {
            layer.alert('Not Found.');
            return;
        }
        editormd.markdownToHTML("body", {
            markdown: result.body,
            tocm: true, // Using [TOCM]
            tocContainer: "#toc-container", // 自定义 ToC 容器层
            htmlDecode: "style,script,iframe", // you can filter tags decode
            emoji: true,
            taskList: true,
            tex: true, // 默认不解析
            flowChart: true, // 默认不解析
            sequenceDiagram: true // 默认不解析
        });

    });

    $('.search_tag').click(function (e) {
        e.preventDefault();
        var key = $(this).text();
        location.href = '/blog/search/?key=' + key + '&page=1';
    });

});

