function renderJoin(j) {
	j.html("");
	const form = $(`<form id="joinForm"></form>`).appendTo(j);
	const div1 = $("<div></div>").appendTo(form);
	const idLabel = $(`<label for="matchID">Match ID:</label>`).appendTo(div1);
	const matchID = $(`<input type="text" autocomplete="off" id="matchID" />`).appendTo(div1);
	const div2 = $("<div></div>").appendTo(form);
	const whiteCheck = $(`<input type="checkbox" checked id="startWhite" />`).appendTo(div2);
	const whiteLabel = $(`<label for="startWhite" />White (create only)</label>`).appendTo(div2);
	const div3 = $("<div></div>").appendTo(form);
	const createButton = $(`<button id="create">Create</button>`).appendTo(div3);
	const joinButton = $(`<button id="join">Join</button>`).appendTo(div3);

	form.on("submit", ev => ev.preventDefault());
	matchID.on("submit", ev => ev.preventDefault());

	createButton.on("click", () => {
		if (matchID.val())
			ws.send(`:Create ${matchID.val()}` + (whiteCheck.is(":checked")? "" : " black"));
	});

	joinButton.on("click", () => {
		if (matchID.val())
			ws.send(":Join " + matchID.val());
	});
}
