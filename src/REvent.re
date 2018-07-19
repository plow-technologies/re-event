type dom;

type event;

type listener = event => unit;

[@bs.val] external document : dom = "document";

[@bs.send] external js_createEvent : (dom, string) => event = "createEvent"; 

[@bs.send] external js_initEvent : (event, string, bool, bool) => unit = "initEvent";

[@bs.send] external js_addEventListener : (dom, string, listener, bool) => listener = "addEventListener";

[@bs.send] external js_removeEventListener : (dom, string, listener) => unit =
  "removeEventListener";

[@bs.send] external js_dispatchEvent : (dom, event) => unit = "dispatchEvent";

[@bs.set] external js_attachPayload : (event, Js.Json.t) => unit = "payload";

[@bs.get] external js_unsafe_getEventPayload : event => Js.null_undefined(Js.Json.t) = "payload";

let emit = (event: event) : unit => js_dispatchEvent(document, event);

/* We return the same listener to easily remove it later using `off` */
let on = (eventType: string, listener: listener) : listener => {
  js_addEventListener(document, eventType, listener, false);
  listener
};

let off = (eventType: string, listener: listener) : unit =>
  js_removeEventListener(document, eventType, listener);

let createEvent = (eventName: string, payload: option(Js.Json.t)) => {
  let ev = js_createEvent(document, "Event");
  js_initEvent(ev, eventName, true, true);
  switch payload {
  | None => ev
  | Some(p) =>
    js_attachPayload(ev, p);
    ev
  }
};

let getPayload = (event: event, parseFunc: Js.Json.t => 'a) : option('a) => {
  let payload = js_unsafe_getEventPayload(event) |> Js.toOption;
  let result =
    switch payload {
    | None => None
    | Some(json) =>
      switch (parseFunc(json)) {
      | obj => Some(obj)
      | exception _ => None
      }
    };
  result
};