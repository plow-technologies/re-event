type dom;

type event;

[@bs.val] external document : dom = "document";

[@bs.send] external createEventLegacy : (dom, string) => event = "createEvent";

[@bs.new] external createEvent : string => event = "Event";

[@bs.send] external addEventListener : (dom, string, event => unit) => unit = "addEventListener";

[@bs.send] external removeEventListener : (dom, string) => unit = "removeEventListener";

[@bs.send] external dispatchEvent : (dom, event) => unit = "dispatchEvent";

[@bs.set] external attachPayload : (event, Js.Json.t) => unit = "payload";

[@bs.get] external unsafe_getEventPayload : event => Js.null_undefined(Js.Json.t) = "payload";

let emit = (event: event) : unit => dispatchEvent(document, event);

let on = (eventName: string, callback: event => unit) =>
  addEventListener(document, eventName, callback);

let getEventPayload = (event: event, parseFunc: Js.Json.t => 'a) : option('a) => {
  let payload = unsafe_getEventPayload(event) |> Js.toOption;
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
